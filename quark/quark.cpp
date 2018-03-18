#include <Windows.h>
#include "Browser.h"

//Applicaition entry point
int WINAPI WinMain(HINSTANCE inst, HINSTANCE prevInst, LPSTR cmdLine, int cmdShow) {
	Browser browser;
	if (!browser.Create("Quark Engine", WS_OVERLAPPEDWINDOW)) {
		return 0;
	}

	ShowWindow(browser.Window(), cmdShow);

	//Message Loop
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
