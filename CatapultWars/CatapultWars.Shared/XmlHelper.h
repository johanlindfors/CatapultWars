#pragma once
#include <ppltasks.h>
#include <d3d11_2.h>
#include "Common\DirectXHelper.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"
#include "Animation.h"
#include <unordered_map>
#include <collection.h>
/*
	std::unordered_map<std::wstring, std::shared_ptr<Animation>>	m_animations;
		std::unordered_map<std::wstring, int>	
*/

concurrency::task<void> ParseXmlAndCreateAnimations(ID3D11Device* device, std::wstring filename, std::unordered_map<std::wstring,int>& splitFrames, std::unordered_map<std::wstring, std::shared_ptr<CatapultWars::Animation>>& animations, bool isLeftSide) {
	try {
		return concurrency::create_task(Windows::ApplicationModel::Package::Current->InstalledLocation->GetFileAsync(ref new Platform::String(filename.c_str())))
			.then([&, device](Windows::Storage::StorageFile^ file) {
			return concurrency::create_task(Windows::Storage::FileIO::ReadTextAsync(file)).then([&, device](Platform::String^ xml) {
				auto doc = ref new Windows::Data::Xml::Dom::XmlDocument();
				doc->LoadXml(xml);

				auto path = "descendant::Definition[@IsAI=\"" + (isLeftSide ? "true" : "false") + "\"]";
				auto definitions = doc->SelectNodes(path);
				for (auto definition : definitions)
				{
					auto animationAlias = definition->Attributes->GetNamedItem("Alias")->InnerText;
					POINT frameSize;
					POINT sheetSize;
					//			TimeSpan frameInterval;
					Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
					DirectX::SimpleMath::Vector2 offset;

					for (auto attribute : definition->Attributes)
					{
						if (attribute->NodeName == "SheetName") {
							auto textureFilename = "Assets\\" + attribute->InnerText + ".png";
							DX::ThrowIfFailed(
								DirectX::CreateWICTextureFromFile(device, textureFilename->Data(), nullptr, texture.ReleaseAndGetAddressOf())
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
							splitFrames[animationAlias->Data()] = _wtol(attribute->InnerText->Data());
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

					auto animation = std::make_shared<CatapultWars::Animation>(texture.Get(), frameSize, sheetSize);
					animation->Offset = offset;
					std::wstring key(animationAlias->Data());
					animations[key] = animation;
				}

			});
		}).then([](concurrency::task<void> t) {
			try {
				t.get();
			}
			catch (...) {

			}
		});
	}
	catch (Platform::Exception^ ex) {

	}
}