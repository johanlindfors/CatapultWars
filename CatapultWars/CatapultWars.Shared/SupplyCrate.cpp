#include "pch.h"
#include <SupplyCrate.h>

using namespace CatapultWars;
using namespace concurrency;
using namespace Windows::Storage;
using namespace Windows::Data::Xml::Dom;

SupplyCrate::SupplyCrate(std::shared_ptr<SpriteBatch>& spriteBatch, String^ idleTextureName, Vector2 catapultCenterPosition, bool isAI)
	: m_lastUpdateState(CrateState::Idle)
{
	m_idleTextureName = idleTextureName;
	m_catapultCenter = catapultCenterPosition;
	m_isAI = isAI;
	m_spriteBatch = spriteBatch;
	m_spriteEffects = SpriteEffects::SpriteEffects_None;
}

task<void> SupplyCrate::Initialize(ID3D11Device* device) {
	m_animationRunning = false;
	m_currentState = CrateState::Idle;

	array<task<void>, 2> tasks = {
		ParseXmlAndCreateAnimations(device),
		create_task([&, device]() {
			ComPtr<ID3D11Resource> res;
			DX::ThrowIfFailed(
				CreateWICTextureFromFile(device, m_idleTextureName->Data(), res.ReleaseAndGetAddressOf(), m_idleTexture.ReleaseAndGetAddressOf())
				);

			DX::GetTextureSize(res.Get(), &m_textureWidth, &m_textureHeight);
			Width = (int)m_textureWidth;
			Height = (int)m_textureHeight;

			int xOffset = m_isAI ? m_positionXOffset : -m_positionXOffset - m_textureWidth;
			Position = m_catapultCenter + Vector2(xOffset, m_positionYOffset);
		})
	};

	return when_all(begin(tasks), end(tasks));
}

task<void> SupplyCrate::ParseXmlAndCreateAnimations(ID3D11Device* device) {
	try {
		return create_task(Windows::ApplicationModel::Package::Current->InstalledLocation->GetFileAsync("Assets\\Textures\\Crate\\AnimationsDef.xml"))
			.then([&, device](Windows::Storage::StorageFile^ file) {
			return create_task(FileIO::ReadTextAsync(file)).then([&, device](String^ xml) {
				XmlDocument^ doc = ref new XmlDocument();
				doc->LoadXml(xml);

				String^ path = L"descendant::Definition";
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


void SupplyCrate::Update(double elapsedSeconds) {
	CrateState postUpdateStateChange;

	// The crate is destroyed, so there is nothing to update
	if (IsDestroyed) {
		return;
	}

	switch (m_currentState)
	{
	case CrateState::Idle:
		// Nothing to do
		break;
	case CrateState::Hit:
		// Progress hit animation
		if (m_animations[L"explode"]->IsActive == false)
		{
			IsDestroyed = true;
		}

		m_animations[L"explode"]->Update();
		break;
	default:
		break;
	}

	m_lastUpdateState = m_currentState;
}

void SupplyCrate::Draw() {
	if (IsDestroyed) {
		return;
	}

	switch (m_currentState)
	{
	case CrateState::Idle:
		m_spriteBatch->Draw(m_idleTexture.Get(), Position, nullptr, Colors::White, 0.0f, Vector2(0, 0), 1.0f, m_spriteEffects, 0);
		break;
	case CrateState::Hit:
		// Progress hit animation
		if (m_animations[L"explode"]->IsActive == false)
		{
			IsDestroyed = true;
		}

		m_animations[L"explode"]->Draw(m_spriteBatch,Position,SpriteEffects::SpriteEffects_None);
		break;
	default:
		break;
	}
}

void SupplyCrate::Hit() {
	AnimationRunning = false;
	m_animations[L"explode"]->PlayFromFrameIndex(0);
	m_currentState = CrateState::Hit;
}