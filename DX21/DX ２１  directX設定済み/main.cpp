/*===========================================================
windowの表示[main.cpp]
           
		                                        Anothor : sasaki yuya
												Date    : 2019/ 05/08
---------------------------------------------------------------------

============================================================*/

#include <Windows.h>


/*------------------------------------------------------------------
	定数定義
-------------------------------------------------------------------*/
#define CLASS_NAME     "GammeWindow"
#define WINDOW_CAPTION "ウィンドウ表示処理"
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)


/*--------------------------------------------------------------------
	プロトタイプ宣言
--------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wPARAM, LPARAM lParam);


/*--------------------------------------------------------------------
	メイン
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
RECT window_rect = { // 矩形というデータ　幅高さではない
	0,0,SCREEN_WIDTH, SCREEN_HEIGHT
};

int windowstile = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);

AdjustWindowRect(&window_rect, windowstile, FALSE);

int window_width  = window_rect.right - window_rect.left;//ウィンドウズの大きさを計算している 
int window_height = window_rect.bottom - window_rect.top;

int desktop_width = GetSystemMetrics(SM_CXSCREEN);
int desktop_height = GetSystemMetrics(SM_CYSCREEN);
int window_x = (desktop_width - window_width) / 2; //あとはこれを下のXYに入れるだけ
int window_y = max((desktop_height - window_height) / 2,0);//同様

HWND hWnd = CreateWindow(
	CLASS_NAME,
	WINDOW_CAPTION,
	windowstile,//　ウィンドウの形を決めている（ｗｓオーバーラップウィンドウ）
	CW_USEDEFAULT ,//X
	CW_USEDEFAULT,//Y
	CW_USEDEFAULT,//幅
	CW_USEDEFAULT,//高さ
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
		//あれを書く
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	else {
		//ゲーム処理
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
			SendMessage(hWnd, WM_CLOSE, 0, 0); // CLOSEへ飛ぶ
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
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



