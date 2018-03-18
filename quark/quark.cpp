#include <Windows.h>
#include <string>
#include <iostream>
#include "ChakraCore.h"

HINSTANCE hinst;

//Function prototypes
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
std::string InitializeChakra();
BOOL InitApplication(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

//handle messages sent to this window
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	PAINTSTRUCT ps;
	HDC hdc;
	std::string res;

	switch (msg) {
		case WM_CREATE: //creation of the window, once this is done lets start up ChakraCore
			return 0;
		case WM_PAINT:
			res = InitializeChakra();
			hdc = BeginPaint(hwnd, &ps);
			TextOut(hdc, 5, 5, res.c_str(), res.size());
			EndPaint(hwnd, &ps);
			return 0;
		case WM_SIZE:
			return 0;
		case WM_DESTROY:
			return 0;
	}
	LONG lRet = DefWindowProc(hwnd, msg, wparam, lparam);
	return lRet;
}

//Applicaition entry point
int WINAPI WinMain(HINSTANCE inst, HINSTANCE prevInst, LPSTR cmdLine, int cmdShow) {
	MSG msg;

	if (!InitApplication(inst)) {
		return FALSE;
	}

	if (!InitInstance(inst, cmdShow)) {
		return FALSE;
	}

	BOOL gotMessage;
	while ((gotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0 && gotMessage != -1) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
	UNREFERENCED_PARAMETER(cmdLine);
}

BOOL InitApplication(HINSTANCE inst) {
	WNDCLASSEX wcx;

	//Fill in the window class structure with parameters
	//that describe the main window

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW; //redraw on resize
	wcx.lpfnWndProc = MainWndProc; //pointer to window procedure
	wcx.cbClsExtra = 0; //give no extra class memory
	wcx.cbWndExtra = 0; //give no extra window memory
	wcx.hInstance = inst; //handle to instance
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION); //predefined app icon
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW); //predefined arrow
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //white background
	wcx.lpszMenuName = "MainMenu"; //name of menu resource
	wcx.lpszClassName = "MainWClass"; //name of window class
	wcx.hIconSm = (HICON)LoadImage(inst, MAKEINTRESOURCE(5), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);

	//Register the window class
	return RegisterClassEx(&wcx);
}

BOOL InitInstance(HINSTANCE inst, int cmdShow) {
	HWND hwnd;
	//Save the application-instance handle
	hinst = inst;
	//create the main window
	hwnd = CreateWindow(
		"MainWClass",
		"Sample",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(HWND)NULL,
		(HMENU)NULL,
		inst,
		(LPVOID)NULL
	);

	if (!hwnd) {
		return FALSE;
	}

	//Show window and send WM_PAINT message to the window proc
	ShowWindow(hwnd, cmdShow);
	UpdateWindow(hwnd);
	return TRUE;
}

std::string InitializeChakra() {
	JsRuntimeHandle runtime;
	JsContextRef context;
	JsValueRef result;
	unsigned currentSourceContext = 0;

	//example script, eventually we will replace this with what is input from the user
	std::wstring script = L"(() => { return \'Hello World!\';})()";

	//create the runtime
	JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime);

	//create our execution context
	JsCreateContext(runtime, &context);

	//set the current exec context
	JsSetCurrentContext(context);
	
	//run our example script
	JsRunScript(script.c_str(), currentSourceContext++, L"", &result);

	//convert return to string even if string
	JsValueRef resultJSString;
	JsConvertValueToString(result, &resultJSString);

	//project script back to C++
	const wchar_t *resultWC;
	size_t stringLen;
	JsStringToPointer(resultJSString, &resultWC, &stringLen);

	std::wstring resultW(resultWC);

	//Dispose of the runtime, eventually we will make this a managed resource under the RAII guidelines for C++14
	JsSetCurrentContext(JS_INVALID_REFERENCE);
	JsDisposeRuntime(runtime);
	return std::string(resultW.begin(), resultW.end());
}