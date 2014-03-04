#include "pch.h"
#include "Catapult.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

using namespace CatapultGame;

Catapult::Catapult(wchar_t* idleTexture, XMFLOAT2 position, SpriteEffects spriteEffect, bool isAi) :
m_winScore(5),
m_gravity(500)
{
	m_idleTextureName = idleTexture;
	m_catapultPosition = position;
	m_spriteEffects = spriteEffect;
	m_isAI = isAi;
}

Catapult::~Catapult()
{

}

void Catapult::Initialize()
{
	IsActive = true;
	AnimationRunning = false;
	m_currentState = CatapultState::Idle;
	m_stallUpdateCycles = 0;

	ParseXmlAndCreateAnimations();

	XMFLOAT2 projectileStartPosition;
	if (m_isAI)
		projectileStartPosition = XMFLOAT2(630, 340);
	else
		projectileStartPosition = XMFLOAT2(175, 340);

	m_projectile = new Projectile();
	m_projectile->Initialize();
}

void Catapult::ParseXmlAndCreateAnimations()
{
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

	switch (m_currentState)
	{
	case CatapultGame::Idle:
		// Nothing to do
		break;
	case CatapultGame::Aiming:
		break;
	case CatapultGame::Firing:
		break;
	case CatapultGame::ProjectileFlying:
		break;
	case CatapultGame::ProjectileHit:
		break;
	case CatapultGame::Hit:
		break;
	case CatapultGame::Reset:
		break;
	case CatapultGame::Stalling:
		break;
	default:
		break;
	}

	m_lastUpdateState = m_currentState;
	if (postUpdateStateChange != 0)
	{
		m_currentState = postUpdateStateChange;
	}
}

bool Catapult::AimReachedShotStrength()
{
	return(m_animations[L"Aim"]->FrameIndex == m_animations[L"Aim"]->FrameCount*ShotStrength);
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
		m_animations[L"fireMiss"]->Draw(m_spriteBatch, m_catapultPosition, m_spriteEffects);
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
	m_currentState = CatapultState::Hit;
}

void Catapult::Fire(float velocity)
{
	m_projectile->Fire(velocity, velocity);
}

bool Catapult::CheckHit()
{
	bool bRes = false;

	XMFLOAT3 center = XMFLOAT3(m_projectile->ProjectilePosition.x, m_projectile->ProjectilePosition.y, 0);
	BoundingSphere* sphere = new BoundingSphere(center,
		max(m_projectile->ProjectileTextureWidth / 2,
		m_projectile->ProjectileTextureHeight / 2));

	// Check Self-Hit - create a bounding box around self
	XMFLOAT3 min = XMFLOAT3(m_catapultPosition.x, m_catapultPosition.y, 0);
	XMFLOAT3 max = XMFLOAT3(
		m_catapultPosition.x + m_animations[L"Fire"]->FrameSize.x,
		m_catapultPosition.y + m_animations[L"Fire"]->FrameSize.y,
		0);
	BoundingBox* selfBox = new BoundingBox(min, max);

	// Check enemy - create a bounding box around the enemy
	min = XMFLOAT3(m_enemy->Catapult->Position.x, m_enemy->Catapult->Position.y, 0);
	max = XMFLOAT3(
		m_enemy->Catapult->Position.x + m_animations[L"Fire"]->FrameSize.x,
		m_enemy->Catapult->Position.y + m_animations[L"Fire"]->FrameSize.y,
		0);
	BoundingBox* enemyBox = new BoundingBox(min, max);

	// Check self hit
	if (sphere->Intersects(selfBox) && m_currentState != CatapultState::Hit)
	{
		//AudioManager.PlaySound("catapultExplosion");

		// Launch hit animation sequence on self
		Hit();
		m_enemy->Score = m_enemy->Score + 1;
		bRes = true;
	}
	// Check if enemy was hit
	else if (sphere->Intersects(enemyBox)
		&& m_enemy->Catapult->CurrentState != CatapultState::Hit
		&& m_enemy->Catapult->CurrentState != CatapultState::Reset)
	{
		//AudioManager.PlaySound("catapultExplosion");

		// Launch enemy hit animaton
		m_enemy->Catapult->Hit();
		m_self->Score = m_enemy->Score + 1;
		bRes = true;
		m_currentState = CatapultState::Reset;
	}

	return bRes;
}

void Catapult::DrawIdleCatapult()
{

}
