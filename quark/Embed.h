#include <Windows.h>
#include <exdisp.h>
#include <string>
#include <comdef.h>

class Embed : public IOleClientSite, public IOleInPlaceSite, public IStorage {
public:
	Embed(HWND parentHwnd) : pHwnd(parentHwnd) {
		//we first need to set our registry to make sure we use IE11 and not IE7!!!!
		if (SetRegistry() == FALSE)
			return;
		refCount = 0;
		::SetRect(&rObj, -300, -300, 300, 300);
		if (CreateBrowser() == FALSE)
			return;
		ShowWindow(GetControlWindow(), SW_SHOW);
		this->Navigate(L"https://github.com/jscherer92/Quark");
	};

	//Methods that we are going to use
	BOOL CreateBrowser();
	RECT PixelToHiMetric(const RECT &rc);
	void SetRect(const RECT &rc);

	void Navigate(std::wstring url);
	//Setup the required classes for the three OLE interfaces
	//// IUnkown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **obj) override;
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	//// IOleWindow
	virtual HRESULT STDMETHODCALLTYPE GetWindow(HWND *hwnd) override;
	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL enterMode) override;

	//// IOleInPlaceSite
	virtual HRESULT STDMETHODCALLTYPE CanInPlaceActivate() override;
	virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivate() override;
	virtual HRESULT STDMETHODCALLTYPE OnUIActivate() override;
	virtual HRESULT STDMETHODCALLTYPE GetWindowContext(IOleInPlaceFrame **frame, IOleInPlaceUIWindow **doc, LPRECT posRect, LPRECT clipRect, LPOLEINPLACEFRAMEINFO framInfo) override;
	virtual HRESULT STDMETHODCALLTYPE Scroll(SIZE extant) override;
	virtual HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL undoable) override;
	virtual HWND GetControlWindow();
	virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate() override;
	virtual HRESULT STDMETHODCALLTYPE DiscardUndoState() override;
	virtual HRESULT STDMETHODCALLTYPE DeactivateAndUndo() override;
	virtual HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT posRect) override;

	//// IOleClientSite
	virtual HRESULT STDMETHODCALLTYPE SaveObject() override;
	virtual HRESULT STDMETHODCALLTYPE GetMoniker(DWORD assign, DWORD whichMoniker, IMoniker **mk) override;
	virtual HRESULT STDMETHODCALLTYPE GetContainer(IOleContainer **container) override;
	virtual HRESULT STDMETHODCALLTYPE ShowObject() override;
	virtual HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL show) override;
	virtual HRESULT STDMETHODCALLTYPE RequestNewObjectLayout() override;

	//// IStorage
	virtual HRESULT STDMETHODCALLTYPE CreateStream(const OLECHAR *name, DWORD mode, DWORD r1, DWORD r2, IStream **stm) override;
	virtual HRESULT STDMETHODCALLTYPE OpenStream(const OLECHAR *name, void *r1, DWORD mode, DWORD r2, IStream **stm) override;
	virtual HRESULT STDMETHODCALLTYPE CreateStorage(const OLECHAR *name, DWORD mode, DWORD r1, DWORD r2, IStorage **stg) override;
	virtual HRESULT STDMETHODCALLTYPE OpenStorage(const OLECHAR *name, IStorage *priority, DWORD mode, SNB exclude, DWORD r, IStorage **stg) override;
	virtual HRESULT STDMETHODCALLTYPE CopyTo(DWORD cExclude, const IID *rExclude, SNB sExclude, IStorage *dest) override;
	virtual HRESULT STDMETHODCALLTYPE MoveElementTo(const OLECHAR *name, IStorage *dest, const OLECHAR *newName, DWORD flags) override;
	virtual HRESULT STDMETHODCALLTYPE Commit(DWORD flags) override;
	virtual HRESULT STDMETHODCALLTYPE Revert() override;
	virtual HRESULT STDMETHODCALLTYPE EnumElements(DWORD r1, void *r2, DWORD r3, IEnumSTATSTG **e) override;
	virtual HRESULT STDMETHODCALLTYPE DestroyElement(const OLECHAR *name) override;
	virtual HRESULT STDMETHODCALLTYPE RenameElement(const OLECHAR *oName, const OLECHAR *nName) override;
	virtual HRESULT STDMETHODCALLTYPE SetElementTimes(const OLECHAR *name, const FILETIME *ctime, const FILETIME *atime, const FILETIME *mtime) override;
	virtual HRESULT STDMETHODCALLTYPE SetClass(REFCLSID id) override;
	virtual HRESULT STDMETHODCALLTYPE SetStateBits(DWORD stateBits, DWORD mask) override;
	virtual HRESULT STDMETHODCALLTYPE Stat(STATSTG *stg, DWORD flag) override;
protected:
	IOleObject* oleObject;
	IOleInPlaceObject* oleInPlaceObject;
	IWebBrowser2* webBrowser2;
	LONG refCount;
	RECT rObj;

	HWND pHwnd;
	HWND cHwnd;
private:
	BOOL SetRegistry();
};