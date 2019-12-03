/*===========================================================
window�̕\��[main.cpp]
           
		                                        Anothor : sasaki yuya
												Date    : 2019/ 05/08
---------------------------------------------------------------------

============================================================*/

#include <Windows.h>


/*------------------------------------------------------------------
	�萔��`
-------------------------------------------------------------------*/
#define CLASS_NAME     "GammeWindow"
#define WINDOW_CAPTION "�E�B���h�E�\������"
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)


/*--------------------------------------------------------------------
	�v���g�^�C�v�錾
--------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wPARAM, LPARAM lParam);


/*--------------------------------------------------------------------
	���C��
--------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
UNREFERENCED_PARAMETER(hPrevInstance);
UNREFERENCED_PARAMETER(lpCmdLine);

WNDCLASS wc = {};
wc.lpfnWndProc = WndProc;
wc.lpszClassName = CLASS_NAME;
wc.hInstance = hInstance;
wc.hCursor = LoadCursor(NULL, IDC_ARROW);
wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

RegisterClass(&wc);
RECT window_rect = { // ��`�Ƃ����f�[�^�@�������ł͂Ȃ�
	0,0,SCREEN_WIDTH, SCREEN_HEIGHT
};

int windowstile = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);

AdjustWindowRect(&window_rect, windowstile, FALSE);

int window_width  = window_rect.right - window_rect.left;//�E�B���h�E�Y�̑傫�����v�Z���Ă��� 
int window_height = window_rect.bottom - window_rect.top;

int desktop_width = GetSystemMetrics(SM_CXSCREEN);
int desktop_height = GetSystemMetrics(SM_CYSCREEN);
int window_x = (desktop_width - window_width) / 2; //���Ƃ͂��������XY�ɓ���邾��
int window_y = max((desktop_height - window_height) / 2,0);//���l

HWND hWnd = CreateWindow(
	CLASS_NAME,
	WINDOW_CAPTION,
	windowstile,//�@�E�B���h�E�̌`�����߂Ă���i�����I�[�o�[���b�v�E�B���h�E�j
	CW_USEDEFAULT ,//X
	CW_USEDEFAULT,//Y
	CW_USEDEFAULT,//��
	CW_USEDEFAULT,//����
	NULL,
	NULL,
	hInstance,
	NULL
);
ShowWindow(hWnd, nCmdShow);
UpdateWindow(hWnd);
 
MSG msg{};
while (WM_QUIT != msg.message) {
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		//���������
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	else {
		//�Q�[������
	}
}
return (int)msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0); // CLOSE�֔��
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



