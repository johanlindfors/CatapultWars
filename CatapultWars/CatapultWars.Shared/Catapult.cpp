#include "pch.h"
#include "Catapult.h"
#include <ppltasks.h>

using namespace CatapultWars;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::Data::Xml;
using namespace Windows::Data::Xml::Dom;
using namespace Windows::Globalization::NumberFormatting;

using namespace concurrency;

Catapult::Catapult(String^ idleTexture, Vector2 position, SpriteEffects spriteEffect, bool isHuman, bool isLeftSide)
	: m_winScore(5)
	, m_gravity(500) 
	, m_isInitialized(false) 
{
	m_idleTextureName = idleTexture;
	m_catapultPosition = position;
	m_spriteEffects = spriteEffect;
	m_isLeftSide = isLeftSide;
	m_isHuman = isHuman;

	if (m_isLeftSide)
		ProjectileStartPosition = Vector2(630, 340);
	else
		ProjectileStartPosition = Vector2(175, 340);
}

task<void> Catapult::Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager) {
	IsActive = true;
	AnimationRunning = false;
	CurrentState = CatapultState::Idle;
	m_stallUpdateCycles = 0;
	m_audioManager = audioManager;

	return ParseXmlAndCreateAnimations(device).then([&,device]() {
		DX::ThrowIfFailed(
			CreateWICTextureFromFile(device, m_idleTextureName->Data(), nullptr, m_idleTexture.ReleaseAndGetAddressOf())
			);

		m_normalProjectile = new Projectile(spriteBatch, L"Assets\\Textures\\Ammo\\rock_ammo.png", ProjectileStartPosition, m_animations[L"Fire"]->FrameSize.y, m_isLeftSide, m_gravity);
		m_normalProjectile->Initialize(device);

		m_splitProjectile = new Projectile(spriteBatch, L"Assets\\Textures\\Ammo\\split_ammo.png", ProjectileStartPosition, m_animations[L"Fire"]->FrameSize.y, m_isLeftSide, m_gravity);
		m_splitProjectile->Initialize(device);

		m_spriteBatch = spriteBatch;

		m_isInitialized = true;
	});
}

task<void> Catapult::ParseXmlAndCreateAnimations(ID3D11Device* device) {
	try {
		return create_task(Windows::ApplicationModel::Package::Current->InstalledLocation->GetFileAsync("Assets\\Textures\\Catapults\\AnimationsDef.xml"))
			.then([&,device](Windows::Storage::StorageFile^ file) {
			return create_task(FileIO::ReadTextAsync(file)).then([&,device](String^ xml) {
				XmlDocument^ doc = ref new XmlDocument();
				doc->LoadXml(xml);

				auto path = "descendant::Definition[@IsAI=\"" + (m_isLeftSide ? "true" : "false") + "\"]";
				auto definitions = doc->SelectNodes(path);
				for (auto definition : definitions)
				{
					String^ animationAlias = definition->Attributes->GetNamedItem("Alias")->InnerText;
					POINT frameSize;
					POINT sheetSize;
					//			TimeSpan frameInterval;
					ComPtr<ID3D11ShaderResourceView> texture;
					Vector2 offset;

					for (auto attribute : definition->Attributes)
					{
						if (attribute->NodeName == "SheetName") {
							auto textureFilename = "Assets\\" + attribute->InnerText + ".png";
							DX::ThrowIfFailed(
								CreateWICTextureFromFile(device, textureFilename->Data(), nullptr, texture.ReleaseAndGetAddressOf())
								);
							continue;
						}
						if (attribute->NodeName == "FrameWidth") {
							frameSize.x = _wtol(attribute->InnerText->Data());
							continue;
						}
						if (attribute->NodeName == "FrameHeight") {
							frameSize.y = _wtol(attribute->InnerText->Data());
							continue;
						}
						if (attribute->NodeName == "SheetColumns") {
							sheetSize.x = _wtol(attribute->InnerText->Data());
							continue;
						}
						if (attribute->NodeName == "SheetRows") {
							sheetSize.y = _wtol(attribute->InnerText->Data());
							continue;
						}
						if (attribute->NodeName == "SplitFrame") {
							m_splitFrames[animationAlias] = _wtol(attribute->InnerText->Data());
							continue;
						}
						//if (attribute->NodeName == "Speed") {
						//	float speed = _wtol(attribute->InnerText->Data());
						//	frameInterval.Duration = 1 / speed * 100;
						//	continue;
						//}
						if (attribute->NodeName == "OffsetX") {
							offset.x = _wtol(attribute->InnerText->Data());
							continue;
						}
						if (attribute->NodeName == "OffsetY") {
							offset.y = _wtol(attribute->InnerText->Data());
							continue;
						}
					}

					auto animation = ref new Animation(texture.Get(), frameSize, sheetSize);
					animation->Offset = offset;
					m_animations[animationAlias] = animation;
				}

			});

		});
	}
	catch (Exception^ ex) {

	}
}

void Catapult::Update(double elapsedSeconds) {
	bool isGroundHit;
	bool startStall;
	CatapultState postUpdateStateChange = CatapultState::Idle;

	if (!IsActive || !m_isInitialized) {
		return;
	}

	switch (CurrentState)
	{
	case CatapultState::Idle:
		// Nothing to do
		break;

	case CatapultState::Aiming:
		if (m_lastUpdateState != CatapultState::Aiming)
		{
			m_audioManager->PlaySound("RopeStretch", true);

			AnimationRunning = true;
			if (m_isLeftSide == true)
			{
				m_animations[L"Aim"]->PlayFromFrameIndex(0);
				m_stallUpdateCycles = 20;
				startStall = false;
			}
		}

		// Progress Aiming "animation"
		if (m_isLeftSide == false)
		{
			UpdateAimAccordingToShotStrength();
		} else
		{
			m_animations[L"Aim"]->Update();
			startStall = AimReachedShotStrength();
			CurrentState = startStall ? CatapultState::Stalling : CatapultState::Aiming;
		}
		break;

	case CatapultState::Stalling:
		if (m_stallUpdateCycles-- <= 0)
		{
			Fire(ShotVelocity);
			postUpdateStateChange = CatapultState::Firing;
		}
		break;

	case CatapultState::Firing:
		// Progress Fire animation
		if (m_lastUpdateState != CatapultState::Firing)
		{
			m_audioManager->StopSound("RopeStretch");
			m_audioManager->PlaySound("CatapultFire");
			StartFiringFromLastAimPosition();
		}

		m_animations[L"Fire"]->Update();

		// If in the "split" point of the animation start
		// projectile fire sequence
		if (m_animations[L"Fire"]->FrameIndex == m_splitFrames[L"Fire"])
		{
			postUpdateStateChange = (CatapultState)(CurrentState | CatapultState::ProjectileFlying);
			m_normalProjectile->ProjectilePosition = m_normalProjectile->ProjectileStartPosition;
		}
		break;

	case (CatapultState::Firing | CatapultState::ProjectileFlying) :
		// Progress Fire animation
		m_animations[L"Fire"]->Update();

		// Update projectile velocity & position in flight
		isGroundHit = m_normalProjectile->UpdateProjectileFlightData(elapsedSeconds, m_wind, m_gravity);
		if (isGroundHit)
		{
			postUpdateStateChange = CatapultState::ProjectileHit;
			m_animations[L"fireMiss"]->PlayFromFrameIndex(0);
		}
		break;

	case CatapultState::ProjectileFlying:
		// Update projectile velocity & position in flight
		isGroundHit = m_normalProjectile->UpdateProjectileFlightData(elapsedSeconds, m_wind, m_gravity);
		if (isGroundHit)
		{
			postUpdateStateChange = CatapultState::ProjectileHit;
			m_animations[L"fireMiss"]->PlayFromFrameIndex(0);
		}
		break;

	case CatapultState::ProjectileHit:
		if (!CheckHit())
		{
			if (m_lastUpdateState != CatapultState::ProjectileHit)
			{
				ApplicationInsights::TrackEvent(L"Miss");
				//VibrateController.Default.Start(TimeSpan.FromMilliseconds(100));
				m_audioManager->PlaySound("BoulderHit");
			}

			// Hit animation finished playing
			if (m_animations[L"fireMiss"]->IsActive == false)
			{
				postUpdateStateChange = CatapultState::Reset;
			}

			m_animations[L"fireMiss"]->Update();
		} else {
			ApplicationInsights::TrackEvent(L"Hit");

			// Catapult hit - start longer vibration on any catapult hit
			// Remember that the call to "CheckHit" updates the catapult's
			// state to "Hit"

			//VibrateController.Default.Start(TimeSpan.FromMilliseconds(500));
		}
		break;

	case CatapultState::HitKill:
		if ((m_animations[L"Destroyed"]->IsActive == false) &&
			(m_animations[L"hitSmoke"]->IsActive == false))
		{
			if (m_enemy->Score >= m_winScore)
			{
				GameOver = true;
				break;
			}

			postUpdateStateChange = CatapultState::Reset;
		}

		m_animations[L"Destroyed"]->Update();
		m_animations[L"hitSmoke"]->Update();

		break;

	case CatapultState::Reset:
		AnimationRunning = false;
		break;

	default:
		break;
	}

	m_lastUpdateState = CurrentState;
	if (postUpdateStateChange != 0)
	{
		CurrentState = postUpdateStateChange;
	}
}

bool Catapult::AimReachedShotStrength() {
	int frameIndex = m_animations[L"Aim"]->FrameIndex;
	int frameCount = m_animations[L"Aim"]->FrameCount;
	int frameShot = frameCount * ShotStrength - 1;
	return(frameIndex == frameShot);
}

void Catapult::UpdateAimAccordingToShotStrength() {
	auto aimAnimation = m_animations[L"Aim"];
	int frameToDisplay = aimAnimation->FrameCount * ShotStrength;
	aimAnimation->FrameIndex = frameToDisplay;
}

void Catapult::StartFiringFromLastAimPosition() {
	int startFrame = m_animations[L"Aim"]->FrameCount -
		m_animations[L"Aim"]->FrameIndex;
	m_animations[L"Fire"]->PlayFromFrameIndex(startFrame);
}

void Catapult::Draw() {

	if (!m_isInitialized) {
		return;
	}

	switch (m_lastUpdateState)
	{
	case CatapultState::Idle:
		DrawIdleCatapult();
		break;
	case CatapultState::Stalling:
	case CatapultState::Aiming:
		m_animations[L"Aim"]->Draw(m_spriteBatch, m_catapultPosition, m_spriteEffects);
		break;
	case CatapultState::Firing:
		m_animations[L"Fire"]->Draw(m_spriteBatch, m_catapultPosition, m_spriteEffects);
		break;
	case CatapultState::Firing | CatapultState::ProjectileFlying:
	case CatapultState::ProjectileFlying:
		m_animations[L"Fire"]->Draw(m_spriteBatch, m_catapultPosition, m_spriteEffects);
		m_normalProjectile->Draw();
		break;
	case CatapultState::ProjectileHit:
		// Draw the catapult
		DrawIdleCatapult();

		// Projectile Hit animation
		m_animations[L"fireMiss"]->Draw(m_spriteBatch, m_normalProjectile->ProjectileHitPosition, m_spriteEffects);
		break;
	case CatapultState::HitKill:
		// Catapult hit animation
		m_animations[L"Destroyed"]->Draw(m_spriteBatch, m_catapultPosition, m_spriteEffects);

		// Projectile smoke animation
		m_animations[L"hitSmoke"]->Draw(m_spriteBatch, m_catapultPosition, m_spriteEffects);
		break;
	case CatapultState::Reset:
		DrawIdleCatapult();
		break;
	default:
		break;
	}
}

void Catapult::Hit() {
	AnimationRunning = true;
	m_animations[L"Destroyed"]->PlayFromFrameIndex(0);
	m_animations[L"hitSmoke"]->PlayFromFrameIndex(0);
	CurrentState = CatapultState::HitKill;
}

void Catapult::Fire(float velocity) {
	m_normalProjectile->Fire(velocity, velocity);
}

bool Catapult::CheckHit() {
	bool bRes = false;

	XMFLOAT3 center = XMFLOAT3(m_normalProjectile->ProjectilePosition.x, m_normalProjectile->ProjectilePosition.y, 0);
	BoundingSphere sphere = BoundingSphere(center,
		max(m_normalProjectile->ProjectileTextureWidth / 2,
			m_normalProjectile->ProjectileTextureHeight / 2));

	// Check Self-Hit - create a bounding box around self
	XMFLOAT3 catapultCenter = XMFLOAT3(
		m_catapultPosition.x + (m_animations[L"Fire"]->FrameSize.x / 2),
		m_catapultPosition.y + (m_animations[L"Fire"]->FrameSize.x / 2), 0);
	XMFLOAT3 extents = XMFLOAT3(
		m_animations[L"Fire"]->FrameSize.x,
		m_animations[L"Fire"]->FrameSize.y,
		0);
	BoundingBox selfBox = BoundingBox(catapultCenter, extents);

	// Check enemy - create a bounding box around the enemy
	catapultCenter = XMFLOAT3(
		m_enemy->Catapult->Position.x + (m_animations[L"Fire"]->FrameSize.x / 2),
		m_enemy->Catapult->Position.y + (m_animations[L"Fire"]->FrameSize.y / 2), 0);
	extents = XMFLOAT3(
		m_animations[L"Fire"]->FrameSize.x,
		m_animations[L"Fire"]->FrameSize.y,
		0);
	BoundingBox enemyBox = BoundingBox(catapultCenter, extents);

	// Check self hit
	if (sphere.Intersects(selfBox) && CurrentState != CatapultState::HitKill)
	{
		m_audioManager->PlaySound("CatapultExplosion");

		// Launch hit animation sequence on self
		Hit();
		m_enemy->Score = m_enemy->Score + 1;
		bRes = true;
	}
	// Check if enemy was hit
	else if (sphere.Intersects(enemyBox)
		&& m_enemy->Catapult->CurrentState != CatapultState::HitKill
		&& m_enemy->Catapult->CurrentState != CatapultState::Reset)
	{
		m_audioManager->PlaySound("CatapultExplosion");

		// Launch enemy hit animaton
		m_enemy->Catapult->Hit();
		m_self->Score = m_self->Score + 1;
		bRes = true;
		CurrentState = CatapultState::Reset;
	}

	return bRes;
}

void Catapult::DrawIdleCatapult() {
	m_spriteBatch->Draw(m_idleTexture.Get(), m_catapultPosition, nullptr, Colors::White, 0.0f, Vector2(0, 0), 1.0f, m_spriteEffects, 0);
}
