#include "pch.h"
#include "Catapult.h"

using namespace std;
using namespace CatapultWars;
using namespace Platform;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;
using namespace concurrency;
using namespace Windows::Data::Xml;
using namespace Windows::Data::Xml::Dom;
using namespace Windows::Storage;

Catapult::Catapult(wstring idleTexture, Vector2 position, SpriteEffects spriteEffect, bool isLeftSide, bool isHuman)
	: m_winScore(5)
	, m_gravity(500)
	, m_idleTextureName(idleTexture)
	, m_catapultPosition(position)
	, m_spriteEffects(spriteEffect)
	, m_isLeftSide(isLeftSide)
	, m_isHuman(isHuman)
{
	GameOver = false;

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

	m_spriteBatch = spriteBatch;

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, m_idleTextureName.c_str(), nullptr, m_idleTexture.ReleaseAndGetAddressOf())
		);


	Vector2 projectileStartPosition;
	if (m_isHuman)
		projectileStartPosition = Vector2(630, 340);
	else
		projectileStartPosition = Vector2(175, 340);

	m_spriteBatch = spriteBatch;

	return ParseXmlAndCreateAnimations(device).then([&, device]() {
		m_normalProjectile = make_shared<Projectile>(spriteBatch, m_activeProjectiles, L"Assets\\Textures\\Ammo\\rock_ammo.png", ProjectileStartPosition, m_animations[L"Fire"]->FrameSize.y, m_isLeftSide, m_gravity);
		m_splitProjectile = make_shared<Projectile>(spriteBatch, m_activeProjectiles, L"Assets\\Textures\\Ammo\\split_ammo.png", ProjectileStartPosition, m_animations[L"Fire"]->FrameSize.y, m_isLeftSide, m_gravity);
		m_crate = make_shared<SupplyCrate>(spriteBatch, L"Assets\\Textures\\Crate\\box.png", GetPosition() + Vector2(m_animations[L"Fire"]->FrameSize.x / 2, 0), m_isLeftSide);
		array<task<void>, 3> tasks =
		{
			m_normalProjectile->Initialize(device),
			m_splitProjectile->Initialize(device),
			m_crate->Initialize(device)
		};
		return when_all(begin(tasks), end(tasks))
			.then([&]() {
				m_isInitialized = true;
			});
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
							m_splitFrames[animationAlias->Data()] = _wtol(attribute->InnerText->Data());
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

					auto animation = make_shared<Animation>(texture.Get(), frameSize, sheetSize);
					animation->Offset = offset;
					wstring key(animationAlias->Data());
					m_animations[key] = animation;
				}

			});

		});
	}
	catch (Exception^ ex) {

	}
}

void Catapult::Update(double elapsedSeconds) {
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
			if (m_isLeftSide == true && ! m_isHuman)
			{
				m_animations[L"Aim"]->PlayFromFrameIndex(0);
				m_stallUpdateCycles = 20;
				startStall = false;
			}
		}

		// Progress Aiming "animation"
		if (m_isHuman) {
			UpdateAimAccordingToShotStrength();
		} else if(m_isLeftSide && !m_isHuman) {
			m_animations[L"Aim"]->Update();
			startStall = AimReachedShotStrength();
			CurrentState = startStall ? CatapultState::Stalling : CatapultState::Aiming;
		}
		break;

	case CatapultState::Stalling:
		if (m_stallUpdateCycles-- <= 0)
		{
			//Fire(ShotVelocity);
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
		if (m_animations[L"Fire"]->GetFrameIndex() == m_splitFrames[L"Fire"])
		{
			Fire(ShotVelocity, ShotAngle);
		} 
		if(m_animations[L"Fire"]->IsActive == false) {
			postUpdateStateChange = CatapultState::ProjectilesFalling;
		}
		break;

	case CatapultState::ProjectilesFalling:
		if (m_activeProjectiles.size() == 0) {
			postUpdateStateChange = CatapultState::Reset;
		}
		break;

	case CatapultState::HitDamage:
		if (m_animations[L"hitSmoke"]->IsActive == false) {
			postUpdateStateChange = CatapultState::Reset;
		}
		m_animations[L"hitSmoke"]->Update();
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

			m_self->Health = 100;
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
	if (postUpdateStateChange != CatapultState::Idle)
	{
		CurrentState = postUpdateStateChange;
	}

	m_destroyedProjectiles.clear();
	m_activeProjectilesCopy.clear();
		
	m_activeProjectilesCopy = vector<shared_ptr<Projectile>>(m_activeProjectiles);

	for (auto projectile : m_activeProjectilesCopy)
	{
		projectile->Update(elapsedSeconds);

		if ((projectile->State == ProjectileState::HitGround) &&
			(projectile->HitHandled == false)) {
			HandleProjectileHit(projectile);
		}
		if (projectile->State == ProjectileState::Destroyed) {
			m_destroyedProjectiles.push_back(projectile);
		}
	}

	for (auto projectile : m_destroyedProjectiles)
	{
		for (int i = 0; i < m_activeProjectiles.size(); i++)
		{
			if (m_activeProjectiles.at(i) == projectile) {
				m_activeProjectiles.erase(m_activeProjectiles.begin()+i);
			}
		}
	}

	m_crate->Update(elapsedSeconds);
}

bool Catapult::AimReachedShotStrength()
{
	int frameIndex = m_animations[L"Aim"]->GetFrameIndex();
	int frameCount = m_animations[L"Aim"]->FrameCount;
	int frameShot = frameCount * ShotStrength - 1;
	return(frameIndex == frameShot);
}

void Catapult::UpdateAimAccordingToShotStrength() {
	auto aimAnimation = m_animations[L"Aim"];
	int frameToDisplay = aimAnimation->FrameCount * ShotStrength;
	aimAnimation->SetFrameIndex(frameToDisplay);
}

void Catapult::StartFiringFromLastAimPosition()
{
	int startFrame = m_animations[L"Aim"]->FrameCount - m_animations[L"Aim"]->GetFrameIndex();
	m_animations[L"Fire"]->PlayFromFrameIndex(startFrame);
}

void Catapult::Draw() {

	if (!m_isInitialized) {
		return;
	}

	switch (m_lastUpdateState)
	{
	case CatapultState::ProjectilesFalling:
	case CatapultState::Idle:
	case CatapultState::Reset:
		DrawIdleCatapult();
		break;
	case CatapultState::Aiming:
	case CatapultState::Stalling:
		m_animations[L"Aim"]->Draw(m_spriteBatch, m_catapultPosition, m_spriteEffects);
		break;
	case CatapultState::Firing:
		m_animations[L"Fire"]->Draw(m_spriteBatch, m_catapultPosition, m_spriteEffects);
		break;	
	case CatapultState::HitDamage:
		// Draw the catapult
		DrawIdleCatapult();
		break;
	case CatapultState::HitKill:
		// Catapult hit animation
		m_animations[L"Destroyed"]->Draw(m_spriteBatch, m_catapultPosition, m_spriteEffects);
		break;
	default:
		break;
	}

	for (auto projectile : m_activeProjectiles) {
		projectile->Draw();
	}

	m_crate->Draw();
}

void Catapult::HandleProjectileHit(shared_ptr<Projectile> projectile) {
	projectile->HitHandled = true;

	switch (CheckHit(projectile))
	{
	case HitCheckResult::SelfCrate:
		// Ignore self crate hits
	case HitCheckResult::Nothing:
		PerformNothingHit(projectile);
		break;

	case HitCheckResult::SelfCatapult:
		if ((CurrentState == CatapultState::HitKill) ||
			(CurrentState == CatapultState::HitDamage)) {
			projectile->HitAnimation = m_animations[L"hitSmoke"];
		}
		break;

	case HitCheckResult::EnemyCatapult:
		if ((m_enemy->Catapult->CurrentState == CatapultState::HitKill) ||
			(m_enemy->Catapult->CurrentState == CatapultState::HitDamage)) {
			projectile->HitAnimation = m_animations[L"hitSmoke"];
		} else {
			PerformNothingHit(projectile);
		}
		break;
	case HitCheckResult::EnemyCrate:
		if (m_enemy->Catapult->GetCrate()->CurrentState == CrateState::Idle) {
			m_audioManager->PlaySound("CatapultExplosion");
			projectile->HitAnimation = m_animations[L"hitSmoke"];
			m_enemy->Catapult->GetCrate()->Hit();
			m_self->Weapon = WeaponType::Split;
		} else {
			PerformNothingHit(projectile);
		}
		break;

	default:
		break;
	}

	if (projectile->HitAnimation != nullptr) {
		projectile->HitAnimation->PlayFromFrameIndex(0);
	}
}

void Catapult::PerformNothingHit(shared_ptr<Projectile> projectile) {
	m_audioManager->PlaySound("BoulderHit");
	projectile->HitAnimation = m_animations[L"fireMiss"];
}

void Catapult::Hit(bool isKilled) {
	AnimationRunning = true;
	if (isKilled) {
		m_animations[L"Destroyed"]->PlayFromFrameIndex(0);
	}

	m_animations[L"hitSmoke"]->PlayFromFrameIndex(0);

	if (isKilled) {
		CurrentState = CatapultState::HitKill;
	} else {
		CurrentState = CatapultState::HitDamage;
	}

	m_self->Weapon = WeaponType::Normal;
}

void Catapult::Fire(float velocity, float angle) {
	std::shared_ptr<Projectile> firedProjectile = nullptr;

	switch (m_self->Weapon)
	{
	case WeaponType::Normal:
		firedProjectile = m_normalProjectile;
		break;
	case WeaponType::Split:
		firedProjectile = m_splitProjectile;
		break;
	default:
		break;
	}
	firedProjectile->ProjectilePosition = firedProjectile->ProjectileStartPosition;
	firedProjectile->Fire(velocity * cos(angle), velocity * sin(angle));
	firedProjectile->Wind = m_wind;
	m_activeProjectiles.push_back(firedProjectile);
}

HitCheckResult Catapult::CheckHit(shared_ptr<Projectile> projectile) {
	HitCheckResult hitRes = HitCheckResult::Nothing;

	XMFLOAT3 center = XMFLOAT3(projectile->ProjectilePosition.x, projectile->ProjectilePosition.y, 0);
	BoundingSphere sphere = BoundingSphere(center,
		max(projectile->ProjectileTextureWidth / 2,
			projectile->ProjectileTextureHeight / 2));

	// Check Self-Hit - create a bounding box around self
	XMFLOAT3 catapultCenter = XMFLOAT3(
		m_catapultPosition.x + (m_animations[L"Fire"]->FrameSize.x / 2),
		m_catapultPosition.y + (m_animations[L"Fire"]->FrameSize.x / 2), 0);
	XMFLOAT3 extents = XMFLOAT3(
		m_animations[L"Fire"]->FrameSize.x,
		m_animations[L"Fire"]->FrameSize.y,
		0);
	BoundingBox selfBox(catapultCenter, extents);

	// Check enemy - create a bounding box around the enemy
	catapultCenter = XMFLOAT3(
		m_enemy->Catapult->GetPosition().x + (m_animations[L"Fire"]->FrameSize.x / 2),
		m_enemy->Catapult->GetPosition().y + (m_animations[L"Fire"]->FrameSize.y / 2), 0);
	extents = XMFLOAT3(
		m_animations[L"Fire"]->FrameSize.x,
		m_animations[L"Fire"]->FrameSize.y,
		0);
	BoundingBox enemyBox(catapultCenter, extents);

	// Check self-crate - Create bounding box around own crate
	auto selfBoxCenter = XMFLOAT3(m_crate->Position.x, m_crate->Position.y, 0);
	auto selfBoxExtents = XMFLOAT3(m_crate->Width, m_crate->Height, 0);
	BoundingBox selfCrateBox(selfBoxCenter, selfBoxExtents);

	// Check enemy-crate - Create bounding box around enemy crate
	auto enemyBoxCenter = XMFLOAT3(m_enemy->Catapult->GetCrate()->Position.x, m_enemy->Catapult->GetCrate()->Position.y, 0);
	auto enemyBoxExtents = XMFLOAT3(m_enemy->Catapult->GetCrate()->Width, m_enemy->Catapult->GetCrate()->Height, 0);
	BoundingBox enemyCrateBox(enemyBoxCenter, enemyBoxExtents);

	// Check self hit
	if (sphere.Intersects(selfBox) && CurrentState != CatapultState::HitKill)
	{
		m_audioManager->PlaySound("CatapultExplosion");

		// Launch hit animation sequence on self
		UpdateHealth(m_self, sphere, selfBox);
		if (m_self->Health <= 0) {
			Hit(true);
			m_enemy->Score = m_enemy->Score + 1;
		}

		hitRes = HitCheckResult::SelfCatapult;
	}
	// Check if enemy was hit
	else if (sphere.Intersects(enemyBox)
		&& m_enemy->Catapult->CurrentState != CatapultState::HitKill
		&& m_enemy->Catapult->CurrentState != CatapultState::Reset)
	{
		m_audioManager->PlaySound("CatapultExplosion");

		// Launch enemy hit animaton
		UpdateHealth(m_enemy, sphere, enemyBox);
		if (m_enemy->Health <= 0) {
			m_enemy->Catapult->Hit(true);
			m_self->Score = m_self->Score + 1;
		}
		hitRes = HitCheckResult::EnemyCatapult;
		CurrentState = CatapultState::Reset;
	} 
	// Check if own crate was hit
	else if (sphere.Intersects(selfCrateBox)) {
		hitRes = HitCheckResult::SelfCrate;
	}
	// Check if enemy crate was hit
	else if (sphere.Intersects(enemyCrateBox)) {
		hitRes = HitCheckResult::EnemyCrate;
	}

	return hitRes;
}

void Catapult::UpdateHealth(Player* player, BoundingSphere projectile, BoundingBox catapult) {
	bool isHit = false;

	float midPoint = catapult.Extents.x / 2;

	//BoundingBox catapultCenter()
}

void Catapult::DrawIdleCatapult() {
	m_spriteBatch->Draw(m_idleTexture.Get(), m_catapultPosition, nullptr, Colors::White, 0.0f, Vector2(0, 0), 1.0f, m_spriteEffects, 0);
}
