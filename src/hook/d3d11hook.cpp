#include "d3d11hook.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <MinHook.h>
#include <hook/BSRenderManager.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>
#include <memory/memory.h>
#include <menu/menu.h>
#include <menu/theme.h>
#include <utils/utils.h>
#include <code_lotd/code_lotd.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace d3d11hook
{
	ID3D11Device* g_pd3dDevice = nullptr;
	IDXGISwapChain* g_pSwapChain = nullptr;
	ID3D11DeviceContext* g_pd3dContext = nullptr;
	ID3D11RenderTargetView* D3D11RenderView = nullptr;

	typedef HRESULT(__stdcall* D3D11PresentFunc)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	D3D11PresentFunc orgD3D11Present2 = nullptr;

	using D3D11PresentFunc2 = HRESULT(STDMETHODCALLTYPE*)(IDXGISwapChain* This, UINT SyncInterval, UINT Flags);

	D3D11PresentFunc2 orgD3D11Present = nullptr;

	decltype(&D3D11CreateDeviceAndSwapChain) orgD3D11CreateDeviceAndSwapChain;

	HRESULT __stdcall newD3D11Present(IDXGISwapChain* This, UINT SyncInterval, UINT Flags)
	{
		static std::once_flag ImguiInit;
		std::call_once(ImguiInit, [&]() {
			g_pSwapChain = This;
			if (SUCCEEDED(g_pSwapChain->GetDevice(IID_PPV_ARGS(&g_pd3dDevice)))) {
				g_pd3dDevice->GetImmediateContext(&g_pd3dContext);
				ID3D11Texture2D* backBuffer;
				g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
				g_pd3dDevice->CreateRenderTargetView(backBuffer, nullptr, &D3D11RenderView);
				backBuffer->Release();
				DXGI_SWAP_CHAIN_DESC sd;
				This->GetDesc(&sd);
				auto window = sd.OutputWindow;
				RECT rect;
				GetClientRect(window, &rect);
				screenWidth = rect.right - rect.left;
				screenHeight = rect.bottom - rect.top;

				//OldWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc));

				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO();
				io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

				menu::initFonts();
				// 初始化缩放
				io.FontGlobalScale = menu::font_scale;

				if (!ImGui_ImplWin32_Init(window)) {
					SKSE::log::warn("ImGui_ImplWin32_Init fail");
				}
				if (!ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dContext)) {
					SKSE::log::warn("ImGui_ImplDX11_Init fail");
				}

				menu::buildStyle();
				SKSE::log::info("ImGui_ImplDX11_Init");
			} else {
				SKSE::log::info("g_pSwapChain->GetDevice(IID_PPV_ARGS(&g_pd3dDevice) Fail");
			}
		});

		//ImGui::GetIO().WantCaptureMouse = active;
		ImGui::GetIO().MouseDrawCursor = active || activeItems;
		//ImGui::GetIO().WantSetMousePos = true;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

#if LOTD_SPECIFIC_CODE
		lotdcode::render();
#else

		menu::render();
#endif

		ImGui::EndFrame();
		ImGui::Render();

		//g_pd3dContext->OMSetRenderTargets(1, &D3D11RenderView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		const auto result = orgD3D11Present(This, SyncInterval, Flags);
		//if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET) {

		//}
		return result;
	}

	bool TryCreateDeviceAndSwapChain()
	{
		HWND hwnd = 0;
		while (true) {
			SKSE::log::info("FindWindow..");
			hwnd = FindWindow(0, "Skyrim Special Edition");
			if (hwnd) {
				break;
			}
			Sleep(2000);
		}

		D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
		DXGI_SWAP_CHAIN_DESC sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;

		//sd.Windowed = ((GetWindowLongPtr(hwnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Windowed = true;

		sd.BufferDesc.Width = 1;
		sd.BufferDesc.Height = 1;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;

		D3D_FEATURE_LEVEL featureLevel{};

		SKSE::log::info("D3D11CreateDeviceAndSwapChain start");
		const auto hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, levels, sizeof(levels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, nullptr);
		SKSE::log::info("D3D11CreateDeviceAndSwapChain end");
		if (FAILED(hr)) {
			SKSE::log::error("D3D11CreateDeviceAndSwapChain Fail {}", (int)hwnd);
			return false;
		}
		return true;
	}

	HRESULT WINAPI newD3D11CreateDeviceAndSwapChain(
		IDXGIAdapter* pAdapter,
		D3D_DRIVER_TYPE DriverType,
		HMODULE Software,
		UINT Flags,
		const D3D_FEATURE_LEVEL* pFeatureLevels,
		UINT FeatureLevels,
		UINT SDKVersion,
		const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
		IDXGISwapChain** ppSwapChain,
		ID3D11Device** ppDevice,
		D3D_FEATURE_LEVEL* pFeatureLevel,
		ID3D11DeviceContext** ppImmediateContext)
	{
		SKSE::log::info("hk_D3D11CreateDeviceAndSwapChain start");
		const auto result = (*orgD3D11CreateDeviceAndSwapChain)(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);

		if (orgD3D11Present == nullptr && ppSwapChain) {
			SKSE::log::info("HookDXGISwapChainPresent ready");
#pragma warning(suppress: 6001)
			auto pVTable = **reinterpret_cast<uintptr_t***>(ppSwapChain);

			orgD3D11Present = reinterpret_cast<D3D11PresentFunc2>(pVTable[8]);

			auto pHook = &newD3D11Present;
			REL::safe_write(reinterpret_cast<uintptr_t>(&pVTable[8]), &pHook, sizeof(uintptr_t));
			SKSE::log::info("HookDXGISwapChainPresent over");
		}

		return result;
	}

	decltype(&D3D11CreateDeviceAndSwapChain) pD3D11CreateDeviceAndSwapChain = nullptr;

	void Install(int type)
	{
		if (type == 1) {
			// 使用BSRenderManager
			auto manager = RE::BSRenderManager::GetSingleton();
			if (!manager) {
				logger::error("Failed to retrieve BSRenderManager");
				return;
			}

			IDXGISwapChain* pswapchain = nullptr;
			while (!pswapchain) {
				pswapchain = reinterpret_cast<IDXGISwapChain*>(manager->swapChain);
				Sleep(1000);
			}

			if (orgD3D11Present == nullptr && pswapchain) {
				SKSE::log::info("HookDXGISwapChainPresent ready");
				auto pVTable = *reinterpret_cast<uintptr_t**>(pswapchain);

				orgD3D11Present = reinterpret_cast<D3D11PresentFunc2>(pVTable[8]);

				auto pHook = &newD3D11Present;
				REL::safe_write(reinterpret_cast<uintptr_t>(&pVTable[8]), &pHook, sizeof(uintptr_t));
				SKSE::log::info("HookDXGISwapChainPresent over");
			}

		} else if (type == 2) {
			// 通常方式
			static std::once_flag D3DInit;
			std::call_once(D3DInit, [&]() {
				if (TryCreateDeviceAndSwapChain()) {
					SKSE::log::info("MH_Initialize");
					if (MH_Initialize() != MH_OK) {
						SKSE::log::error("MH_Initialize Fail");
						return 1;
					}

					DWORD_PTR* pSwapChainVTable = nullptr;
					pSwapChainVTable = (DWORD_PTR*)(g_pSwapChain);
					pSwapChainVTable = (DWORD_PTR*)(pSwapChainVTable[0]);

					SKSE::log::info("MH_CreateHook pSwapChainVTable start");
					if (MH_CreateHook((DWORD_PTR*)pSwapChainVTable[8], newD3D11Present, (LPVOID*)(&orgD3D11Present)) != MH_OK) {
						SKSE::log::error("MH_CreateHook pSwapChainVTable Fail");
						return 1;
					}
					SKSE::log::info("MH_CreateHook pSwapChainVTable success");
					if (MH_EnableHook((DWORD_PTR*)pSwapChainVTable[8]) != MH_OK) {
						SKSE::log::error("MH_EnableHook pSwapChainVTable Fail");
						return 1;
					}
					SKSE::log::info("MH_EnableHook pSwapChainVTable success");
					return 0;
				} else {
					return 0;
				}
			});
		}

		else if (type == 3) {
			// 直接iat
			char* ptr = nullptr;
			auto moduleBase = (uintptr_t)GetModuleHandle(ptr);
			auto dllD3D11 = GetModuleHandleA("d3d11.dll");
			*(FARPROC*)&orgD3D11CreateDeviceAndSwapChain = GetProcAddress(dllD3D11, "D3D11CreateDeviceAndSwapChain");
			SKSE::log::info("GetProcAddress: " + std::to_string((int)orgD3D11CreateDeviceAndSwapChain));
			Detours::IATHook(moduleBase, "d3d11.dll", "D3D11CreateDeviceAndSwapChain", (uintptr_t)newD3D11CreateDeviceAndSwapChain);
		}
	}
}
