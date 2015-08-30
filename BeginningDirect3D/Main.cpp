#include <Windows.h>

WNDPROC WndProc = 0;

int WINAPI wWinMain (
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPWSTR cmdLine,
	int cmdShow
) {
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "DX11WindowClass";

	if (!RegisterClassEx(&wndClass))
		return -1;

	RECT rc = {0, 0, 640, 480};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowA(
		"DX11WindowClass",
		"Blank 32 Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);


	if (!hwnd)
		return -1;

	ShowWindow(hwnd, cmdShow);


	return 0;
}
