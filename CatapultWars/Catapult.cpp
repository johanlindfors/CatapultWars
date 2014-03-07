#include "pch.h"
#include "Catapult.h"

using namespace CatapultWars;

Catapult::Catapult(wchar_t* idleTexture, Vector2 position, SpriteEffects spriteEffect, bool isAi) :
m_winScore(5),
m_gravity(500)
{
	m_idleTextureName = idleTexture;
	m_catapultPosition = position;
	m_spriteEffects = spriteEffect;
	m_isAI = isAi;
}

void Catapult::Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch)
{
	IsActive = true;
	AnimationRunning = false;
	CurrentState = CatapultState::Idle;
	m_stallUpdateCycles = 0;

	ParseXmlAndCreateAnimations(device);

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, m_idleTextureName, nullptr, m_idleTexture.ReleaseAndGetAddressOf())
		);

	Vector2 projectileStartPosition;
	if (m_isAI)
		projectileStartPosition = Vector2(630, 340);
	else
		projectileStartPosition = Vector2(175, 340);

	m_projectile = new Projectile(spriteBatch, L"Assets\\Textures\\Ammo\\rock_ammo.dds", projectileStartPosition, m_animations[L"Fire"]->FrameSize.y, m_isAI, m_gravity);
	m_projectile->Initialize(device);

	m_spriteBatch = spriteBatch;
}

void Catapult::CreateAnimation(ID3D11Device* device, wchar_t* key, wchar_t* textureFilename, int frameWidth, int frameHeight, int sheetColumns, int sheetRows, int splitFrame, int offsetX, int offsetY)
{
	ComPtr<ID3D11ShaderResourceView> texture;
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, textureFilename, nullptr, texture.ReleaseAndGetAddressOf())
		);
	POINT frameSize = { frameWidth, frameHeight };
	POINT sheetSize = { sheetColumns, sheetRows };
	auto animation = new Animation(texture.Get(), frameSize, sheetSize);
	m_animations[key] = animation;
	m_splitFrames[key] = splitFrame;
}

void Catapult::ParseXmlAndCreateAnimations(ID3D11Device* device)
{
	if (m_isAI){
		CreateAnimation(device, L"Fire", L"Assets\\Textures\\Catapults\\Red\\redFire\\redCatapult_fire.dds",
			75, 60, 15, 2, 20, 0, 0);
		CreateAnimation(device, L"Aim", L"Assets\\Textures\\Catapults\\Red\\redPullback\\redCatapult_Pullback.dds",
			75, 60, 18, 1, 0, 0, 0);
		CreateAnimation(device, L"Destroyed", L"Assets\\Textures\\Catapults\\Red\\redDestroyed\\redCatapult_destroyed.dds",
			122, 62, 15, 2, 20, -11, 0);
		CreateAnimation(device, L"fireMiss", L"Assets\\Textures\\Catapults\\Fire_Miss\\fire_miss.dds",
			90, 80, 15, 2, 0, -50, 0);
		CreateAnimation(device, L"hitSmoke", L"Assets\\Textures\\Catapults\\Hit_Smoke\\smoke.dds",
			128, 128, 15, 2, 20, -30, -64);
	}
	else {
		CreateAnimation(device, L"Fire", L"Assets\\Textures\\Catapults\\Blue\\blueFire\\blueCatapult_fire.dds",
			75, 60, 15, 2, 20, 0, 0);
		CreateAnimation(device, L"Aim", L"Assets\\Textures\\Catapults\\Blue\\bluePullback\\blueCatapult_Pullback.dds",
			75, 60, 18, 1, 0, 0, 0);
		CreateAnimation(device, L"Destroyed", L"Assets\\Textures\\Catapults\\Blue\\blueDestroyed\\blueCatapult_destroyed.dds",
			122, 62, 15, 2, 20, -40, 0);
		CreateAnimation(device, L"fireMiss", L"Assets\\Textures\\Catapults\\Fire_Miss\\fire_miss.dds",
			90, 80, 15, 2, 0, -50, 0);
		CreateAnimation(device, L"hitSmoke", L"Assets\\Textures\\Catapults\\Hit_Smoke\\smoke.dds",
			128, 128, 15, 2, 20, 0, -64);
	}
	//// Load multiple animations form XML definition
	//XDocument doc = XDocument.Load("Content/Textures/Catapults/AnimationsDef.xml");
	//XName name = XName.Get("Definition");
	//var definitions = doc.Document.Descendants(name);



	//// Loop over all definitions in XML
	//foreach(var animationDefinition in definitions)
	//{
	//	bool ? toLoad = null;
	//	bool val;
	//	if (bool.TryParse(animationDefinition.Attribute("IsAI").Value, out val))
	//		toLoad = val;

	//	// Check if the animation definition need to be loaded for current catapult
	//	if (toLoad == isAI || null == toLoad)
	//	{
	//		// Get a name of the animation
	//		string animatonAlias = animationDefinition.Attribute("Alias").Value;
	//		Texture2D texture =
	//			curGame.Content.Load<Texture2D>(animationDefinition.Attribute("SheetName").Value);

	//		// Get the frame size (width & height)
	//		Point frameSize = new Point();
	//		frameSize.X = int.Parse(animationDefinition.Attribute("FrameWidth").Value);
	//		frameSize.Y = int.Parse(animationDefinition.Attribute("FrameHeight").Value);

	//		// Get the frames sheet dimensions
	//		Point sheetSize = new Point();
	//		sheetSize.X = int.Parse(animationDefinition.Attribute("SheetColumns").Value);
	//		sheetSize.Y = int.Parse(animationDefinition.Attribute("SheetRows").Value);

	//		// If definition has a "SplitFrame" - means that other animation should start here - load it
	//		if (null != animationDefinition.Attribute("SplitFrame"))
	//			splitFrames.Add(animatonAlias,
	//			int.Parse(animationDefinition.Attribute("SplitFrame").Value));

	//		// Defing animation speed
	//		TimeSpan frameInterval = TimeSpan.FromSeconds((float)1 /
	//			int.Parse(animationDefinition.Attribute("Speed").Value));

	//		Animation animation = new Animation(texture, frameSize, sheetSize);

	//		// If definition has an offset defined - means that it should be rendered relatively
	//		// to some element/other animation - load it
	//		if (null != animationDefinition.Attribute("OffsetX") &&
	//			null != animationDefinition.Attribute("OffsetY"))
	//		{
	//			animation.Offset = new Vector2(int.Parse(animationDefinition.Attribute("OffsetX").Value),
	//				int.Parse(animationDefinition.Attribute("OffsetY").Value));
	//		}

	//		animations.Add(animatonAlias, animation);
	//	}
	//}
}

void Catapult::Update(float timeTotal, float timeDelta)
{
	bool isGroundHit;
	bool startStall;
	CatapultState postUpdateStateChange = CatapultState::Idle;

	if (!IsActive){
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
			//AudioManager::PlaySound(L"ropeStretch", true);

			AnimationRunning = true;
			if (m_isAI == true)
			{
				m_animations[L"Aim"]->PlayFromFrameIndex(0);
				m_stallUpdateCycles = 20;
				startStall = false;
			}
		}

		// Progress Aiming "animation"
		if (m_isAI == false)
		{
			UpdateAimAccordingToShotStrength();
		}
		else
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
			//AudioManager::StopSound(L"ropeStretch");
			//AudioManager::PlaySound(L"catapultFire");
			StartFiringFromLastAimPosition();
		}

		m_animations[L"Fire"]->Update();

		// If in the "split" point of the animation start
		// projectile fire sequence
		if (m_animations[L"Fire"]->FrameIndex == m_splitFrames[L"Fire"])
		{
			postUpdateStateChange = (CatapultState)(CurrentState | CatapultState::ProjectileFlying);
			m_projectile->ProjectilePosition = m_projectile->ProjectileStartPosition;
		}
		break;

	case (CatapultState::Firing | CatapultState::ProjectileFlying) :
		// Progress Fire animation
		m_animations[L"Fire"]->Update();

		// Update projectile velocity & position in flight
		isGroundHit = m_projectile->UpdateProjectileFlightData(timeTotal, timeDelta, m_wind, m_gravity);
		if (isGroundHit)
		{
			postUpdateStateChange = CatapultState::ProjectileHit;
			m_animations[L"fireMiss"]->PlayFromFrameIndex(0);
		}
		break;

	case CatapultState::ProjectileFlying:
		// Update projectile velocity & position in flight
		isGroundHit = m_projectile->UpdateProjectileFlightData(timeTotal, timeDelta, m_wind, m_gravity);
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
				//VibrateController.Default.Start(TimeSpan.FromMilliseconds(100));
				//AudioManager::PlaySound(L"boulderHit");
			}

			// Hit animation finished playing
			if (m_animations[L"fireMiss"]->IsActive == false)
			{
				postUpdateStateChange = CatapultState::Reset;
			}

			m_animations[L"fireMiss"]->Update();
		}
		else {
			// Catapult hit - start longer vibration on any catapult hit
			// Remember that the call to "CheckHit" updates the catapult's
			// state to "Hit"

			//VibrateController.Default.Start(TimeSpan.FromMilliseconds(500));
		}
		break;

	case CatapultState::Hit:
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

bool Catapult::AimReachedShotStrength()
{
	int frameIndex = m_animations[L"Aim"]->FrameIndex;
	int frameCount = m_animations[L"Aim"]->FrameCount;
	int frameShot = frameCount * ShotStrength - 1;
	return(frameIndex == frameShot);
}

void Catapult::UpdateAimAccordingToShotStrength()
{
	auto aimAnimation = m_animations[L"Aim"];
	int frameToDisplay = aimAnimation->FrameCount * ShotStrength;
	aimAnimation->FrameIndex = frameToDisplay;
}

void Catapult::StartFiringFromLastAimPosition()
{
	int startFrame = m_animations[L"Aim"]->FrameCount -
		m_animations[L"Aim"]->FrameIndex;
	m_animations[L"Fire"]->PlayFromFrameIndex(startFrame);
}

void Catapult::Draw()
{
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
		m_projectile->Draw();
		break;
	case CatapultState::ProjectileHit:
		// Draw the catapult
		DrawIdleCatapult();

		// Projectile Hit animation
		m_animations[L"fireMiss"]->Draw(m_spriteBatch, m_projectile->ProjectileHitPosition, m_spriteEffects);
		break;
	case CatapultState::Hit:
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

void Catapult::Hit()
{
	AnimationRunning = true;
	m_animations[L"Destroyed"]->PlayFromFrameIndex(0);
	m_animations[L"hitSmoke"]->PlayFromFrameIndex(0);
	CurrentState = CatapultState::Hit;
}

void Catapult::Fire(float velocity)
{
	m_projectile->Fire(velocity, velocity);
}

bool Catapult::CheckHit()
{
	bool bRes = false;

	XMFLOAT3 center = XMFLOAT3(m_projectile->ProjectilePosition.x, m_projectile->ProjectilePosition.y, 0);
	BoundingSphere sphere = BoundingSphere(center,
		max(m_projectile->ProjectileTextureWidth / 2,
		m_projectile->ProjectileTextureHeight / 2));

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
	if (sphere.Intersects(selfBox) && CurrentState != CatapultState::Hit)
	{
		//AudioManager.PlaySound("catapultExplosion");

		// Launch hit animation sequence on self
		Hit();
		m_enemy->Score = m_enemy->Score + 1;
		bRes = true;
	}
	// Check if enemy was hit
	else if (sphere.Intersects(enemyBox)
		&& m_enemy->Catapult->CurrentState != CatapultState::Hit
		&& m_enemy->Catapult->CurrentState != CatapultState::Reset)
	{
		//AudioManager.PlaySound("catapultExplosion");

		// Launch enemy hit animaton
		m_enemy->Catapult->Hit();
		m_self->Score = m_self->Score + 1;
		bRes = true;
		CurrentState = CatapultState::Reset;
	}

	return bRes;
}

void Catapult::DrawIdleCatapult()
{
	m_spriteBatch->Draw(m_idleTexture.Get(), m_catapultPosition, nullptr, Colors::White, 0.0f, Vector2(0, 0), 1.0f, m_spriteEffects, 0);
}
