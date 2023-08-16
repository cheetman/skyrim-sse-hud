#include "d3d11hook.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <MinHook.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace d3d11hook
{
	ID3D11Device* D3D11Device = nullptr;
	IDXGISwapChain* DXGISwapChain = nullptr;
	ID3D11DeviceContext* D3D11DeviceContext = nullptr;
	ID3D11RenderTargetView* D3D11RenderView = nullptr;

	HWND TargetWindow = nullptr;

	typedef HRESULT(__stdcall* D3D11PresentHook)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	D3D11PresentHook phookD3D11Present = nullptr;


	using WndProcFunc = std::add_pointer_t<LRESULT((__stdcall)(HWND, UINT, WPARAM, LPARAM))>;
	WndProcFunc OldWndProc;

	LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
	}


	HRESULT __stdcall PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		static std::once_flag ImguiInit;
		std::call_once(ImguiInit, [&]() {
			DXGISwapChain = pSwapChain;
			if (SUCCEEDED(DXGISwapChain->GetDevice(IID_PPV_ARGS(&D3D11Device)))) {
				D3D11Device->GetImmediateContext(&D3D11DeviceContext);
				DXGI_SWAP_CHAIN_DESC sd;
				DXGISwapChain->GetDesc(&sd);
				TargetWindow = sd.OutputWindow;
				ID3D11Texture2D* backBuffer;
				DXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
				D3D11Device->CreateRenderTargetView(backBuffer, nullptr, &D3D11RenderView);
				backBuffer->Release();
				OldWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(TargetWindow, GWLP_WNDPROC, (LONG_PTR)WndProc));

				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO();
				io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
				ImGui_ImplWin32_Init(TargetWindow);
				ImGui_ImplDX11_Init(D3D11Device, D3D11DeviceContext);
			}
		});

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();
		ImGui::Begin("TestWindow");
		ImGui::Text("Test.");
		ImGui::End();

		ImGui::EndFrame();
		ImGui::Render();

		D3D11DeviceContext->OMSetRenderTargets(1, &D3D11RenderView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return phookD3D11Present(pSwapChain, SyncInterval, Flags);
	}



	bool TryD3D11()
	{
		auto hd = FindWindow(0, "Skyrim Special Edition");

		DXGI_SWAP_CHAIN_DESC swapChainDesc{};
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.OutputWindow = hd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Windowed = TRUE;

		D3D_FEATURE_LEVEL featureLevel{};

		//using TD3D11CreateDeviceAndSwapChain = HRESULT (*)(IDXGIAdapter * pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels,
		//	UINT FeatureLevels, UINT SDKVersion, const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, IDXGISwapChain** ppSwapChain, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel,
		//	ID3D11DeviceContext** ppImmediateContext);
		//TD3D11CreateDeviceAndSwapChain RealD3D11CreateDeviceAndSwapChain = nullptr;
		//RealD3D11CreateDeviceAndSwapChain = reinterpret_cast<TD3D11CreateDeviceAndSwapChain>(SKSE::PatchIAT(HookD3D11CreateDeviceAndSwapChain, "d3d11.dll", "D3D11CreateDeviceAndSwapChain"));

		const auto hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc, &DXGISwapChain, &D3D11Device, &featureLevel, nullptr);

		if (FAILED(hr)) {
			return false;
		}
		return true;
	}

	void Install()
	{
		static std::once_flag D3DInit;
		std::call_once(D3DInit, [&]() {
			if (TryD3D11()) {
				if (MH_Initialize() != MH_OK) {
					return 1;
				}

				DWORD_PTR* pSwapChainVTable = nullptr;
				pSwapChainVTable = (DWORD_PTR*)(DXGISwapChain);
				pSwapChainVTable = (DWORD_PTR*)(pSwapChainVTable[0]);

				if (MH_CreateHook((DWORD_PTR*)pSwapChainVTable[8], PresentHook, reinterpret_cast<void**>(&phookD3D11Present)) != MH_OK) {
					return 1;
				}
				if (MH_EnableHook((DWORD_PTR*)pSwapChainVTable[8]) != MH_OK) {
					return 1;
				}
			} else {
				return 0;
			}
		});

	}
}
