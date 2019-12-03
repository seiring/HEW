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
RECT window_rect = {
	0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, FALSE);

HWND hWnd = CreateWindow(
	CLASS_NAME,
	WINDOW_CAPTION,
	WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,//�@�E�B���h�E�̌`�����߂Ă���i�����I�[�o�[���b�v�E�B���h�E�j
	CW_USEDEFAULT,//X
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
while (GetMessage(&msg, NULL, 0, 0)) 
{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
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



