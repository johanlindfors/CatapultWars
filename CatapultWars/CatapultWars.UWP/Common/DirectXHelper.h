#pragma once

#include <ppltasks.h>	// For create_task
#include <DDSTextureLoader.h>

namespace DX
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// Set a breakpoint on this line to catch Win32 API errors.
			throw Platform::Exception::CreateException(hr);
		}
	}

	// Function that reads from a binary file asynchronously.
	inline Concurrency::task<std::vector<byte>> ReadDataAsync(const std::wstring& filename)
	{
		using namespace Windows::Storage;
		using namespace Concurrency;

		auto folder = Windows::ApplicationModel::Package::Current->InstalledLocation;

		return create_task(folder->GetFileAsync(Platform::StringReference(filename.c_str()))).then([] (StorageFile^ file) 
		{
			return FileIO::ReadBufferAsync(file);
		}).then([] (Streams::IBuffer^ fileBuffer) -> std::vector<byte> 
		{
			std::vector<byte> returnBuffer;
			returnBuffer.resize(fileBuffer->Length);
			Streams::DataReader::FromBuffer(fileBuffer)->ReadBytes(Platform::ArrayReference<byte>(returnBuffer.data(), fileBuffer->Length));
			return returnBuffer;
		});
	}

	// Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
	inline float ConvertDipsToPixels(float dips, float dpi)
	{
		static const float dipsPerInch = 96.0f;
		return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
	}

	inline void GetTextureSize(ID3D11Resource* res, UINT* width, UINT* height)
	{
		assert(res != 0);

		// This is the most generic solution. you can make it a lot
		// simpler if you know it will always be a 2D texture file
		D3D11_RESOURCE_DIMENSION dim;
		res->GetType(&dim);
		switch (dim)
		{
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			auto txt = reinterpret_cast<ID3D11Texture1D*>(res);
			D3D11_TEXTURE1D_DESC desc;
			txt->GetDesc(&desc);
			if (width) *width = desc.Width;
			if (height) *height = 1;
		}
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			auto txt = reinterpret_cast<ID3D11Texture2D*>(res);
			D3D11_TEXTURE2D_DESC desc;
			txt->GetDesc(&desc);
			if (width) *width = desc.Width;
			if (height) *height = desc.Height;
		}
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			auto txt = reinterpret_cast<ID3D11Texture3D*>(res);
			D3D11_TEXTURE3D_DESC desc;
			txt->GetDesc(&desc);
			if (width) *width = desc.Width;
			if (height) *height = desc.Height;
		}
			break;
		default:
			if (width) *width = 0;
			if (height) *height = 0;
			break;
		}
	}

#if defined(_DEBUG)
	// Check for SDK Layer support.
	inline bool SdkLayersAvailable()
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
			0,
			D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
			nullptr,                    // Any feature level will do.
			0,
			D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
			nullptr,                    // No need to keep the D3D device reference.
			nullptr,                    // No need to know the feature level.
			nullptr                     // No need to keep the D3D device context reference.
			);

		return SUCCEEDED(hr);
	}
#endif
}
