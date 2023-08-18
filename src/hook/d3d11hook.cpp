#include "d3d11hook.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <MinHook.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace d3d11hook
{
	ID3D11Device* g_pd3dDevice = nullptr;
	IDXGISwapChain* g_pSwapChain = nullptr;
	ID3D11DeviceContext* g_pd3dContext = nullptr;
	ID3D11RenderTargetView* D3D11RenderView = nullptr;
	HWND g_hwnd = nullptr;
	bool active = false;

	typedef HRESULT(__stdcall* D3D11PresentHook)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	D3D11PresentHook phookD3D11Present = nullptr;


	using WndProcFunc = std::add_pointer_t<LRESULT((__stdcall)(HWND, UINT, WPARAM, LPARAM))>;
	WndProcFunc OldWndProc;

	LRESULT __stdcall WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{

		//LRESULT ret = 0;
		//if (active) {
		//	//extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
		//	ret = ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		//}
		//return ret;


		//switch (uMsg) {
		//case WM_LBUTTONDOWN:
		//	ImGui::GetIO().MouseDown[0] = true;
		//	return ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		//	break;
		//case WM_LBUTTONUP:
		//	ImGui::GetIO().MouseDown[0] = false;
		//	return ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		//	break;
		//case WM_RBUTTONDOWN:
		//	ImGui::GetIO().MouseDown[1] = true;
		//	return ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		//	break;
		//case WM_RBUTTONUP:
		//	ImGui::GetIO().MouseDown[1] = false;
		//	return ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		//	break;
		//case WM_MBUTTONDOWN:
		//	ImGui::GetIO().MouseDown[2] = true;
		//	return ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		//	break;
		//case WM_MBUTTONUP:
		//	ImGui::GetIO().MouseDown[2] = false;
		//	return ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		//	break;
		//case WM_MOUSEWHEEL:
		//	ImGui::GetIO().MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
		//	return ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		//	break;
		//case WM_MOUSEMOVE:
		//	ImGui::GetIO().MousePos.x = (signed short)(lParam);
		//	ImGui::GetIO().MousePos.y = (signed short)(lParam >> 16);
		//	return ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		//	break;
		//}

		//return CallWindowProc(OldWndProc, hwnd, uMsg, wParam, lParam);




		//if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) {
		//	return true;
		//}
		//return CallWindowProc(OldWndProc, hwnd, uMsg, wParam, lParam);


		return OldWndProc( hwnd, uMsg, wParam, lParam);
	}


	HRESULT __stdcall PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		static std::once_flag ImguiInit;
		std::call_once(ImguiInit, [&]() {
			g_pSwapChain = pSwapChain;
			if (SUCCEEDED(g_pSwapChain->GetDevice(IID_PPV_ARGS(&g_pd3dDevice)))) {
				g_pd3dDevice->GetImmediateContext(&g_pd3dContext);

				ID3D11Texture2D* backBuffer;
				g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
				g_pd3dDevice->CreateRenderTargetView(backBuffer, nullptr, &D3D11RenderView);
				backBuffer->Release();

				
				DXGI_SWAP_CHAIN_DESC sd;
				pSwapChain->GetDesc(&sd);
				auto window = sd.OutputWindow;

				OldWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc));

				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO();
				io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
				ImGui_ImplWin32_Init(g_hwnd);
				ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dContext);
			}
		});

		if (GetAsyncKeyState(VK_INSERT) & 0x1) {
			active ? active = false : active = true;
		}
		//ImGui::GetIO().WantCaptureMouse = active;
		ImGui::GetIO().MouseDrawCursor = active;
		//ImGui::GetIO().WantSetMousePos = true;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		
		

		ImGui::NewFrame();

		if (active) {
			ImGui::Begin("TestWindow",&active);
			ImGui::Text("Test.");
			ImGui::End();
		}


		ImGui::EndFrame();

		ImGui::Render();

		g_pd3dContext->OMSetRenderTargets(1, &D3D11RenderView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return phookD3D11Present(pSwapChain, SyncInterval, Flags);
	}



	bool TryD3D11()
	{
		g_hwnd = FindWindow(0, "Skyrim Special Edition");

		DXGI_SWAP_CHAIN_DESC sd{};
		sd.BufferCount = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.OutputWindow = g_hwnd;
		sd.SampleDesc.Count = 1;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Windowed = ((GetWindowLongPtr(g_hwnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

		D3D_FEATURE_LEVEL featureLevel{};

		const auto hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, nullptr);

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
				pSwapChainVTable = (DWORD_PTR*)(g_pSwapChain);
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
