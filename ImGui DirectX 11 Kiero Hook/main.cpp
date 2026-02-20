#include "includes.h"

#include <xmmintrin.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("ImGui Window");
	if (ImGui::Button("SERA"))
	{
		/**/
	}
	ImGui::End();

	ImGui::Render();
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			//kiero::bind(8, (void**)&oPresent, hkPresent);

			uint64_t BaseAddr = (uint64_t)GetModuleHandleA(nullptr);
			uint64_t tempAddr;

			// typedef int(__cdecl* tget_game_build)();
			// tget_game_build get_game_build = (tget_game_build)((uintptr_t)GetModuleHandleA("citizen-scripting-core.dll") + 0x50170);

			// int g_gameBuild = get_game_build();
			int g_gameBuild = 2372;

			std::stringstream ss;
			ss << "C:\\Offs\\" << g_gameBuild << ".txt";
			std::ofstream file;
			file.open(ss.str());
			tempAddr = CustomAPII::ScanSignature(BaseAddr, "48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81");
			tempAddr = tempAddr + (*reinterpret_cast<signed int*>(tempAddr + 3)) + 7;
			file << "{E(\"WorldPTR\") , 0x" << std::hex << tempAddr - BaseAddr << "},\n";
			
			tempAddr = CustomAPII::ScanSignature(BaseAddr, "48 8D 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 48 8D 0D ? ? ? ? 8A D8 E8 ? ? ? ? 84 DB 75 13 48 8D 0D"); // 48 8D 0D ? ? ? ? 89 44 24 30 E8 ? ? ? ? 48 83 C4 28 C3 48 8B 05 <-- Minha
			tempAddr = tempAddr + (*reinterpret_cast<signed int*>(tempAddr + 3)) + 7;
			file << "{E(\"Replay Interface\") , 0x" << std::hex << tempAddr-BaseAddr << "},\n";
			
			file << "{E(\"W2S\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 89 5C 24 ? 55 56 57 48 83 EC 70 65 4C 8B 0C 25")-BaseAddr << "},\n";
			file << "{E(\"Bone Pos\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 60 48 8B 01 41 8B E8 48 8B F2 48 8B F9 33 DB")-BaseAddr << "},\n";
			
			tempAddr = CustomAPII::ScanSignature(BaseAddr, "48 8B 05 ? ? ? ? 4A 8B 1C F0");
			tempAddr = tempAddr + (*reinterpret_cast<signed int*>(tempAddr + 3)) + 7;
			file << "{E(\"Camera\") , 0x" << std::hex << tempAddr-BaseAddr << "},\n";
						
			tempAddr = CustomAPII::ScanSignature(BaseAddr, "48 8B 15 ?? ?? ?? ?? 48 8D 2D ?? ?? ?? ?? 48 8B CD");
			tempAddr = tempAddr + (*reinterpret_cast<signed int*>(tempAddr + 3)) + 7;
			file << "{E(\"ViewPort\") , 0x" << std::hex << tempAddr-BaseAddr << "},\n";

			tempAddr = CustomAPII::ScanSignature(BaseAddr, "4C 8D 05 ? ? ? ? 0F B7 C1");
			tempAddr = tempAddr + (*reinterpret_cast<signed int*>(tempAddr + 3)) + 7;
			file << "{E(\"Waypoint\") , 0x" << std::hex << tempAddr -BaseAddr << "},\n";

			tempAddr = CustomAPII::ScanSignature(BaseAddr, "48 8B 05 ? ? ? ? 48 8B 1C F8 8B");
			tempAddr = tempAddr + (*reinterpret_cast<signed int*>(tempAddr + 3)) + 7;
			file << "{E(\"Pickpdata\") , 0x" << std::hex << tempAddr-BaseAddr << "},\n";

			tempAddr = CustomAPII::ScanSignature(BaseAddr, "4C 8D 05 ? ? ? ? 4D 8B 08 4D 85 C9 74 11");
			tempAddr = tempAddr + (*reinterpret_cast<signed int*>(tempAddr + 3)) + 7;
			file << "{E(\"Global\") , 0x" << std::hex << tempAddr - BaseAddr << "},\n";
			file << "\n//---------------------------------------- Natives Now -----------------------------------\n\n";
			file << "{E(\"Pointer To Handle\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 8B 15 ? ? ? ? 48 8B F9 48 83 C1 10 33 DB")-BaseAddr << "},\n";
			file << "{E(\"Disable Control Action\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 41 8A F0 8B DA 8B F9")-BaseAddr << "},\n";
			file << "{E(\"Set Vehicle Fixed\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "40 53 48 83 EC 20 E8 ? ? ? ? 48 8B D8 48 85 C0 74 28 4C 8B 10")-BaseAddr << "},\n";
			file << "{E(\"Set Vehicle Deformation Fixed\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 89 5C 24 ? 57 48 83 EC 40 48 83 B9 ? ? ? ? ? 48 8B D9")-BaseAddr << "},\n";
			file << "{E(\"Set Plate Index\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "40 53 48 83 EC 20 48 8B DA E8 ? ? ? ? 48 85 C0 74 10 48 8B 48 48")-BaseAddr << "},\n";
			file << "{E(\"Get Player Name\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "40 53 48 83 EC 20 80 3D ? ? ? ? ? 8B D9 74 22")-BaseAddr << "},\n";
			file << "{E(\"Network Request Control Of Entity\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 89 5C 24 ? 57 48 83 EC 20 8B D9 E8 ? ? ? ? 84 C0")-BaseAddr << "},\n";
			file << "{E(\"Network Has Control Of Entity\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 83 EC 28 E8 ? ? ? ? 48 85 C0 74 1F 48 8B 88 ? ? ? ? 48 85 C9 74 0F")-BaseAddr << "},\n";
			file << "{E(\"Toggle Vehicle Mod\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 89 5C 24 ? 57 48 83 EC 20 41 8A F8 8B DA 83 FA 0A")-BaseAddr << "},\n";
			file << "{E(\"Set Vehicle Mod\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC 30 41 8A E9")-BaseAddr << "},\n";
			file << "{E(\"Get Num Vehicle Mod\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "40 53 48 83 EC 20 8B DA 83 FA 0A 7E 10 8D 42 F5 83 F8 0D 77 05 83 C3 19 EB 03 83 EB 0E E8 ? ? ? ? 48 85 C0 74 2C")-BaseAddr << "},\n";
			file << "{E(\"Request Model\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 89 5C 24 ? 48 89 7C 24 ? 55 48 8B EC 48 83 EC 50 8B 45 18")-BaseAddr << "},\n";
			file << "{E(\"Has Model Loaded\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 89 7C 24 ? 55 48 8B EC 48 83 EC 20 8B 45 18 BF")-BaseAddr << "},\n";
			file << "{E(\"Clear Ped Blood\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "40 53 48 83 EC 20 8A 91 ? ? ? ? 48 8B D9 80 FA FF 74 51")-BaseAddr << "},\n";
			file << "{E(\"Set Vehicle Undriveable\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "40 53 48 83 EC 20 8A DA E8 ? ? ? ? 48 85 C0 74 20 84 DB")-BaseAddr << "},\n";
			file << "{E(\"Create Vehicle\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 50 F3 0F 10 02")-BaseAddr << "},\n";
			file << "{E(\"Is A Valid Model\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 83 EC 28 8B 44 24 38 48 8D 54 24 ? C7 44 24 ? ? ? ? ? 0D ? ? ? ? 25 ? ? ? ? 89 44 24 38 E8 ? ? ? ? 0F B7 44 24 ? 66 89 44 24 ? 8B 44 24 38 0D ? ? ? ? 0F BA F0 1C 0F B7 C8")-BaseAddr << "},\n";
			file << "{E(\"Set Vehicle Forwad Speed\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "40 53 48 83 EC 40 0F 29 74 24 ? 48 8B D9 0F 28 F1 E8 ? ? ? ? 48 8B 03")-BaseAddr << "},\n";
			file << "{E(\"Set Local Player Visible Locally\") , 0x" << std::hex << CustomAPII::ScanSignature(BaseAddr, "48 89 5C 24 ? 57 48 83 EC 30 40 8A F9 E8 ? ? ? ? 48 8B D8 48 85 C0 74 6D")-BaseAddr << "},\n";
			file.close();
			TerminateProcess(GetCurrentProcess(), 0);
			kiero::shutdown();

			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		/*AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);*/

		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}