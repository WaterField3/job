#undef UNICODE  // Unicode�ł͂Ȃ��A�}���`�o�C�g�������g��

#include <Windows.h>

#include "direct3d.h"
#include "Input.h"
#include "Application/Application.h"
#include "GameObject/GameObject.h"
#include "Component/Component.h"
#include "Component/Transform/Transform.h"
#include "GameObjectManager/GameObjectManager.h"

// �}�N����`
#define CLASS_NAME    "DX21Smpl"// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME  "�X�P���g���v���O����"// �E�B���h�E�̖��O

#define SCREEN_WIDTH (1024)	// �E�C���h�E�̕�
#define SCREEN_HEIGHT (576)	// �E�C���h�E�̍���

extern TMF::Application* CreateApplication();

// �֐��̃v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// �G���g���|�C���g����ԍŏ��Ɏ��s�����֐�
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
	D3D::Get()->Create(hWnd);

	// Input�N���X�̏�����
	Input::Get()->Initialize(hWnd, hInstance);

	// FPS�\���p�ϐ�
	int fpsCounter = 0;
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



	

	auto app = CreateApplication();
	app->OnInitialize();

	// �Q�[�����[�v
	for (;;)
	{
		BOOL isAnyMessage = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

		if (isAnyMessage)// �������b�Z�[�W�����邩����
		{
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				break;
			}
		}
		else
		{
			// 1/60�b�o�߂������H
			QueryPerformanceCounter(&liWork);
			nowCount = liWork.QuadPart; // ���ݎ��Ԃ��擾�i�P�ʁF�J�E���g�j
			if (nowCount >= oldCount + numCount_1frame)
			{
				// �Q�[���������s
				Input::Get()->Update();

				app->OnUpdate();

				fpsCounter++; // �Q�[�����[�v���s�񐔂��J�E���g�{�P
				oldCount = nowCount;
			}

			nowTick = GetTickCount64(); // ���ݎ��Ԏ擾
			// �P�b�o�߂������H
			if (nowTick >= oldTick + 1000)
			{
				// FPS��\������
				char str[32];
				wsprintfA(str, "FPS=%d", fpsCounter);
				SetWindowTextA(hWnd, str);
				// �J�E���^�[�����Z�b�g
				fpsCounter = 0;
				oldTick = nowTick;
			}
		}
	} // �Q�[�����[�v�̕��J�b�R

	app->OnFinalize();

	UnregisterClass(CLASS_NAME, hInstance);

	return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:// �E�B���h�E�j���̃��b�Z�[�W
		PostQuitMessage(0);// �gWM_QUIT�h���b�Z�[�W�𑗂�@���@�A�v���I��
		break;

	case WM_CLOSE:  // x�{�^���������ꂽ��
		DestroyWindow(hWnd);  // �gWM_DESTROY�h���b�Z�[�W�𑗂�
		break;

	// �L�[�������ꂽ�C�x���g
	case WM_KEYDOWN:
		break;

	// �L�[�������ꂽ�C�x���g
	case WM_KEYUP:
		break;

	default:
		// ���case�ȊO�̏ꍇ�̏��������s
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}