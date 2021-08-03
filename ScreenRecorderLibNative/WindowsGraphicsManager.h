#pragma once

#include <inspectable.h>
#include <winrt/Windows.Graphics.Capture.h>
#include "TextureManager.h"
#include "CommonTypes.h"

class WindowsGraphicsManager
{
public:
	WindowsGraphicsManager();
	~WindowsGraphicsManager();
	void Close();
	winrt::Windows::Graphics::Capture::GraphicsCaptureItem CaptureItem() { return m_item; }
	HRESULT Initialize(_In_ DX_RESOURCES *Data, _In_ winrt::Windows::Graphics::Capture::GraphicsCaptureItem Output, _In_ bool isCursorCaptureEnabled, _In_ winrt::Windows::Graphics::DirectX::DirectXPixelFormat pixelFormat);
	HRESULT ProcessFrame(_Inout_ GRAPHICS_FRAME_DATA *pData, _Inout_ ID3D11Texture2D *pSharedSurf, _In_  INT OffsetX, _In_  INT OffsetY, _In_ RECT &destinationRect, _In_opt_ const std::optional<RECT> &sourceRect = std::nullopt);
	HRESULT GetFrame(_Inout_ GRAPHICS_FRAME_DATA *pData);
	SIZE ItemSize();
private:
	inline void CheckClosed()
	{
		if (m_closed.load() == true)
		{
			throw winrt::hresult_error(RO_E_CLOSED);
		}
	}
	void CleanRefs();
	HRESULT BlankFrame(_Inout_ ID3D11Texture2D *pSharedSurf, _In_ RECT rect, _In_ INT OffsetX, _In_  INT OffsetY);
private:
	winrt::Windows::Graphics::DirectX::DirectXPixelFormat m_PixelFormat;
	winrt::Windows::Graphics::Capture::GraphicsCaptureItem m_item;
	winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool m_framePool;
	winrt::Windows::Graphics::Capture::GraphicsCaptureSession m_session;

	bool m_HaveDeliveredFirstFrame;
	std::unique_ptr<TextureManager> m_TextureManager;
	std::atomic<bool> m_closed;
	RECT m_LastFrameRect;
	ID3D11Device *m_Device;
	ID3D11DeviceContext *m_DeviceContext;
};