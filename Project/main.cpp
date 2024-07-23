#undef UNICODE  // Unicodeではなく、マルチバイト文字を使う

#include <Windows.h>

#include "direct3d.h"
#include "Input.h"
#include "Application/Application.h"
#include "GameObject/GameObject.h"
#include "Component/Component.h"
#include "Component/Transform/Transform.h"
#include "GameObjectManager/GameObjectManager.h"

// マクロ定義
#define CLASS_NAME    "DX21Smpl"// ウインドウクラスの名前
#define WINDOW_NAME  "スケルトンプログラム"// ウィンドウの名前

#define SCREEN_WIDTH (1024)	// ウインドウの幅
#define SCREEN_HEIGHT (576)	// ウインドウの高さ

extern TMF::Application* CreateApplication();

// 関数のプロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// エントリポイント＝一番最初に実行される関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	auto& gameObjectManager = TMF::GameObjectManager::Instance();
	auto pGameObject = gameObjectManager.CreateGameObject();
	if (auto pObject = pGameObject.lock())
	{
		auto component = pObject-> AddComponent<TMF::Component>();
		auto component2 = pObject->AddComponent<TMF::Transform>();
		auto transcomponent = pObject->GetComponent<TMF::Transform>();
		pObject->RemoveComponent<TMF::Transform>();
		gameObjectManager.DestroyGameObject(pObject.get());
	}



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

	// ゲームループに入る前に
	// DirectXの初期化をする
	D3D::Get()->Create(hWnd);

	// Inputクラスの初期化
	Input::Get()->Initialize(hWnd, hInstance);

	// FPS表示用変数
	int fpsCounter = 0;
	long long oldTick = GetTickCount64();//現在時間を保存
	long long nowTick = oldTick; // 現在時間取得用

	// FPS固定用変数
	LARGE_INTEGER liWork; // 関数から値を受け取る用
	long long frequency; // 計測精度
	// 計測精度を取得
	QueryPerformanceFrequency(&liWork);
	frequency = liWork.QuadPart; // １秒あたりの解像度が入る
	// １フレームあたりのカウント値を計算
	long long numCount_1frame = frequency / 60; // 60FPSで計算
	// 現在時間（単位：カウント）を取得
	QueryPerformanceCounter(&liWork);
	long long oldCount = liWork.QuadPart;
	long long nowCount = oldCount;

	MSG msg;



	

	auto app = CreateApplication();
	app->OnInitialize();

	// ゲームループ
	for (;;)
	{
		BOOL isAnyMessage = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

		if (isAnyMessage)// 何かメッセージがあるか判定
		{
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				break;
			}
		}
		else
		{
			// 1/60秒経過したか？
			QueryPerformanceCounter(&liWork);
			nowCount = liWork.QuadPart; // 現在時間を取得（単位：カウント）
			if (nowCount >= oldCount + numCount_1frame)
			{
				// ゲーム処理実行
				Input::Get()->Update();

				app->OnUpdate();

				fpsCounter++; // ゲームループ実行回数をカウント＋１
				oldCount = nowCount;
			}

			nowTick = GetTickCount64(); // 現在時間取得
			// １秒経過したか？
			if (nowTick >= oldTick + 1000)
			{
				// FPSを表示する
				char str[32];
				wsprintfA(str, "FPS=%d", fpsCounter);
				SetWindowTextA(hWnd, str);
				// カウンターをリセット
				fpsCounter = 0;
				oldTick = nowTick;
			}
		}
	} // ゲームループの閉じカッコ

	app->OnFinalize();

	UnregisterClass(CLASS_NAME, hInstance);

	return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:// ウィンドウ破棄のメッセージ
		PostQuitMessage(0);// “WM_QUIT”メッセージを送る　→　アプリ終了
		break;

	case WM_CLOSE:  // xボタンが押されたら
		DestroyWindow(hWnd);  // “WM_DESTROY”メッセージを送る
		break;

	// キーが押されたイベント
	case WM_KEYDOWN:
		break;

	// キーが離されたイベント
	case WM_KEYUP:
		break;

	default:
		// 上のcase以外の場合の処理を実行
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}