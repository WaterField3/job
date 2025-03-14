#undef UNICODE  // Unicodeではなく、マルチバイト文字を使う

#include <Windows.h>
#include <Imgui/imgui.h>
#include <cereal/cereal.hpp>
#include <fstream>
#include <Keyboard.h>
#include <Mouse.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "direct3d.h"
#include "Application/Application.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/CerealExtention.h"
#include "System/DataFileNames.h"
#include "PhysicsManager.h"
#include "Input.h"
#include "Timer.h"
#include "EffectManager.h"
#include "AudioManager.h"

// マクロ定義
#define CLASS_NAME    "DX21Smpl"// ウインドウクラスの名前
#define WINDOW_NAME  "the Battle"// ウィンドウの名前

#define SCREEN_WIDTH (1024)	// ウインドウの幅
#define SCREEN_HEIGHT (576)	// ウインドウの高さ
using namespace TMF;

extern std::unique_ptr<TMF::Application> CreateApplication();

// 関数のプロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


// エントリポイント＝一番最初に実行される関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd;
	hWnd = CreateWindowEx(0,// 拡張ウィンドウスタイル
		CLASS_NAME,// ウィンドウクラスの名前
		WINDOW_NAME,// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,// ウィンドウスタイル
		CW_USEDEFAULT,// ウィンドウの左上Ｘ座標
		CW_USEDEFAULT,// ウィンドウの左上Ｙ座標 
		SCREEN_WIDTH,// ウィンドウの幅
		SCREEN_HEIGHT,// ウィンドウの高さ
		NULL,// 親ウィンドウのハンドル
		NULL,// メニューハンドルまたは子ウィンドウID
		hInstance,// インスタンスハンドル
		NULL);// ウィンドウ作成データ

	// 指定されたウィンドウの表示状態を設定(ウィンドウを表示)
	ShowWindow(hWnd, nCmdShow);
	// ウィンドウの状態を直ちに反映(ウィンドウのクライアント領域を更新)
	UpdateWindow(hWnd);

	MSG msg;

	// ゲームループに入る前に
	// DirectXの初期化をする
	Timer::Instance().Initialize();
	D3D::Get()->Create(hWnd);

	EffectManager::Instance().Initialize();
	AudioManager::Instance().Initialize();
	// Inputクラスの初期化
	Input::Instance().Initialize(hWnd);
	GameObjectManager::Instance().Load(TITLE_DATA);
	PhysicsManager::Instance().Initialize();

	auto app = CreateApplication();
	app->OnInitialize();
	D3D::Get()->Init();

	int result = MessageBox(NULL,"フルスクリーンにしますか？", "確認", MB_YESNO | MB_ICONQUESTION);
	if (result == IDYES) {
		// Yesが押された場合の処理
		BOOL state = FALSE;
		D3D::Get()->ChangeSwapChainFullScreenState(state);
	}
	else {
		// Noが押された場合の処理
		BOOL state = TRUE;
		D3D::Get()->ChangeSwapChainFullScreenState(state);
	}
	// ゲームループ
	for (;;)
	{
		BOOL isAnyMessage = PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE);

		if (isAnyMessage)// 何かメッセージがあるか判定
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			Timer::Instance().Update();

			// ゲーム処理実行


			app->OnUpdate();

			app->OnDraw();

			app->OnDrawImGui();



			if (Input::Instance().GetKeyState().Escape == true)
			{
				break;
			}
		}
	} // ゲームループの閉じカッコ

	app->OnFinalize();
	EffectManager::Instance().Finalize();
	UnregisterClass(CLASS_NAME, hInstance);

	return (int)msg.wParam;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_DESTROY:// ウィンドウ破棄のメッセージ
		PostQuitMessage(0);// “WM_QUIT”メッセージを送る　→　アプリ終了
		break;

	case WM_CLOSE:  // xボタンが押されたら
		DestroyWindow(hWnd);  // “WM_DESTROY”メッセージを送る
		break;

	case WM_ACTIVATEAPP:
		DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
		DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
		break;

	case WM_ACTIVATE:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
		break;

	case WM_DPICHANGED:
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			//const int dpi = HIWORD(wParam);
			//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
			const RECT* suggested_rect = (RECT*)lParam;
			::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
		break;

	default:
		// 上のcase以外の場合の処理を実行
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}