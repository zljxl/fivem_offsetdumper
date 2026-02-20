#pragma once
#include <process.h>
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <sstream>
#include <array>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstring>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "Memory.hpp"
#include "Class.h"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;