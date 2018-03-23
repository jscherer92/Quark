#include "Browser.h"

LRESULT Browser::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT: {
			std::string test = chakra.RunTestScript();
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(m_hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			TextOut(hdc, 5, 5, test.c_str(), test.size());
			EndPaint(m_hwnd, &ps);
		}
		return 0;
	case WM_SIZE: {
		if (embeddedBrowser != 0)
		{
			RECT rcClient;
			GetClientRect(m_hwnd, &rcClient);
		
			RECT rc;
			rc.left = 0;
			rc.top = 45;
			rc.right = rcClient.right;
			rc.bottom = rcClient.bottom;
			embeddedBrowser->SetRect(rc);
		}
	}
	default:
		return DefWindowProc(m_hwnd, msg, wParam, lParam);
	}
	return TRUE;
}

void Browser::EmbedBrowser() {
	RECT rcClient;
	GetClientRect(m_hwnd, &rcClient);

	embeddedBrowser = new Embed(m_hwnd);
	if (embeddedBrowser != nullptr) {
		RECT rc;
		rc.left = 0;
		rc.top = 45;
		rc.right = rcClient.right;
		rc.bottom = rcClient.bottom;
		embeddedBrowser->SetRect(rc);
	}
}