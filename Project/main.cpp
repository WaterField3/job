#undef UNICODE  // Unicode�ł͂Ȃ��A�}���`�o�C�g�������g��

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

// �}�N����`
#define CLASS_NAME    "DX21Smpl"// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME  "��"// �E�B���h�E�̖��O

#define SCREEN_WIDTH (1024)	// �E�C���h�E�̕�
#define SCREEN_HEIGHT (576)	// �E�C���h�E�̍���
using namespace TMF;

extern std::unique_ptr<TMF::Application> CreateApplication();

// �֐��̃v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


// �G���g���|�C���g����ԍŏ��Ɏ��s�����֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �u���b�N�ԍ����L�^
	//_CrtSetBreakAlloc(3886); // �Y���ԍ��ɓ��B����ƃu���[�N�|�C���g�Œ�~
	//_CrtSetBreakAlloc(2727); // �Y���ԍ��ɓ��B����ƃu���[�N�|�C���g�Œ�~

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
	hWnd = CreateWindowEx(0,// �g���E�B���h�E�X�^�C��
		CLASS_NAME,// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,// �E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,// �E�B���h�E�̍���w���W
		CW_USEDEFAULT,// �E�B���h�E�̍���x���W 
		SCREEN_WIDTH,// �E�B���h�E�̕�
		SCREEN_HEIGHT,// �E�B���h�E�̍���
		NULL,// �e�E�B���h�E�̃n���h��
		NULL,// ���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,// �C���X�^���X�n���h��
		NULL);// �E�B���h�E�쐬�f�[�^

	// �w�肳�ꂽ�E�B���h�E�̕\����Ԃ�ݒ�(�E�B���h�E��\��)
	ShowWindow(hWnd, nCmdShow);
	// �E�B���h�E�̏�Ԃ𒼂��ɔ��f(�E�B���h�E�̃N���C�A���g�̈���X�V)
	UpdateWindow(hWnd);

	// �Q�[�����[�v�ɓ���O��
	// DirectX�̏�����������

	// Input�N���X�̏�����

	// FPS�\���p�ϐ�
	int fpsCounter = 0;
	float elapsedTime = 0.0f;
	float fps = 0.0f;
	long long oldTick = GetTickCount64();//���ݎ��Ԃ�ۑ�
	long long nowTick = oldTick; // ���ݎ��Ԏ擾�p

	// FPS�Œ�p�ϐ�
	LARGE_INTEGER liWork; // �֐�����l���󂯎��p
	long long frequency; // �v�����x
	// �v�����x���擾
	QueryPerformanceFrequency(&liWork);
	frequency = liWork.QuadPart; // �P�b������̉𑜓x������
	// �P�t���[��������̃J�E���g�l���v�Z
	long long numCount_1frame = frequency / 60; // 60FPS�Ōv�Z
	// ���ݎ��ԁi�P�ʁF�J�E���g�j���擾
	QueryPerformanceCounter(&liWork);
	long long oldCount = liWork.QuadPart;
	long long nowCount = oldCount;

	MSG msg;
	Timer::Instance().Initialize();
	D3D::Get()->Create(hWnd);

	EffectManager::Instance().Initialize();
	AudioManager::Instance().Initialize();
	Input::Instance().Initialize(hWnd);
	GameObjectManager::Instance().Load(TITLE_DATA);
	//GameObjectManager::Instance().Load(MAIN_DATA);
	//GameObjectManager::Instance().Load(RESULT_DATA);
	PhysicsManager::Instance().Initialize();

	auto app = CreateApplication();
	app->OnInitialize();
	D3D::Get()->Init();
	// �Q�[�����[�v
	for (;;)
	{
		BOOL isAnyMessage = PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE);

		if (isAnyMessage)// �������b�Z�[�W�����邩����
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
			// �Q�[���������s

			app->OnUpdate();

			app->OnDraw();

			app->OnDrawImGui();

			//// 1/60�b�o�߂������H
			//QueryPerformanceCounter(&liWork);
			//nowCount = liWork.QuadPart; // ���ݎ��Ԃ��擾�i�P�ʁF�J�E���g�j
			//if (nowCount >= oldCount + numCount_1frame)
			//{
			//	// �Q�[���������s

			//	app->OnUpdate();

			//	app->OnDraw();

			//	app->OnDrawImGui();

			//	fpsCounter++; // �Q�[�����[�v���s�񐔂��J�E���g�{�P
			//	oldCount = nowCount;
			//}

			//nowTick = GetTickCount64(); // ���ݎ��Ԏ擾
			//// �P�b�o�߂������H
			//if (nowTick >= oldTick + 1000)
			//{
				// FPS��\������
				//char str[32];
				//wsprintfA(str, "FPS=%d", targetFrameTime);
				//SetWindowTextA(hWnd, str);
			//	// �J�E���^�[�����Z�b�g
			//	fpsCounter = 0;
			//	oldTick = nowTick;
			//}
		}
	} // �Q�[�����[�v�̕��J�b�R

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
	case WM_DESTROY:// �E�B���h�E�j���̃��b�Z�[�W
		PostQuitMessage(0);// �gWM_QUIT�h���b�Z�[�W�𑗂�@���@�A�v���I��
		break;

	case WM_CLOSE:  // x�{�^���������ꂽ��
		DestroyWindow(hWnd);  // �gWM_DESTROY�h���b�Z�[�W�𑗂�
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
		// ���case�ȊO�̏ꍇ�̏��������s
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}