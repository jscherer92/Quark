#include <Windows.h>
#include <exdisp.h>
#include <MsHTML.h>
#include "Embed.h"

template <class T>
class BaseBrowser {
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		T *This = NULL;

		if (msg == WM_NCCREATE) {
			CREATESTRUCT* create = (CREATESTRUCT*)lParam;
			This = (T*)create->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)This);

			This->m_hwnd = hwnd;
		}
		else {
			This = (T*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		if (This) {
			return This->HandleMessage(msg, wParam, lParam);
		}
		else {
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}

	BaseBrowser() : m_hwnd(NULL) {}

	BOOL Create(
		LPCSTR windowName,
		DWORD style,
		DWORD exStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int width = CW_USEDEFAULT,
		int height = CW_USEDEFAULT,
		HWND wndParent = 0,
		HMENU menu = 0
		) {
		WNDCLASS wc = { 0 };
		
		wc.lpfnWndProc = T::WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = ClassName();

		RegisterClass(&wc);

		m_hwnd = CreateWindowEx(exStyle, ClassName(), windowName, style, x, y, width, height, wndParent, menu, GetModuleHandle(NULL), this);

		return (m_hwnd ? TRUE : FALSE);
	}

	HWND Window() const { return m_hwnd; }

protected:
	virtual LPCSTR ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;
};

class Browser : public BaseBrowser<Browser>
{
public:
	Browser() {
		oleInitializeSuccess = SUCCEEDED(OleInitialize(NULL));
	}
	~Browser() {
		OleUninitialize();
	}
	LPCSTR ClassName() const { return "Quark"; }
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	void EmbedBrowser();
protected:
	Embed *embeddedBrowser;
private:
	BOOL oleInitializeSuccess;
};