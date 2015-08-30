#include <Windows.h>

// Forward Declaration
LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
);

//---------------------------------------------------------------------------------------
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

	// Register a window class with name "DX11WindowClass"
	if (!RegisterClassEx(&wndClass))
		return -1;

	RECT rc = {0, 0, 640, 480};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// Now create the window from the previously registered class "DX11WindowClass"
	HWND hwnd = CreateWindowA(
		"DX11WindowClass",
		"Blank Direct3D Window",
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

	
	MSG msg = { 0 };

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			// Translates input virtual key messages to ascii char key messages.
			TranslateMessage(&msg);

			// Disptach message to our registered Window Procedure Callback function.
			DispatchMessage(&msg);
		} 
		else {
			// Update
			// Draw
		}
	}


	return static_cast<int>(msg.wParam);
}

//---------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
) {
	PAINTSTRUCT paintStruct;
	HDC hDC;

	switch (message) {
	case WM_PAINT:
		//-- Draw window background:
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;

	case WM_DESTROY: 
		// Send a WM_QUIT message to terminate our application loop within wWinMain().
		PostQuitMessage(0);
		break;

	default:
		// Pass unprocessed messages to the default windows procedure callback.
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}
