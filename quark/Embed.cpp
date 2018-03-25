#include "Embed.h"

BOOL Embed::CreateBrowser() {
	HRESULT hr;
	hr = ::OleCreate(CLSID_WebBrowser, IID_IOleObject, OLERENDER_DRAW, 0, this, this, (void**)&oleObject);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Can't create oleObject CLSID_WebBrowser", "Error", MB_ICONERROR);
		return FALSE;
	}

	hr = oleObject->SetClientSite(this);
	hr = OleSetContainedObject(oleObject, TRUE);

	RECT posRect;
	::SetRect(&posRect, -300, -300, 300, 300);
	hr = oleObject->DoVerb(OLEIVERB_INPLACEACTIVATE, NULL, this, -1, pHwnd, &posRect);
	if (FAILED(hr))
	{
		MessageBox(NULL, "oleObject->DoVerb failed", "Error", MB_ICONERROR);
		return FALSE;
	}

	hr = oleObject->QueryInterface(&webBrowser2);
	if (FAILED(hr))
	{
		MessageBox(NULL, "oleObject->QueryInterface failed", "Error", MB_ICONERROR);
		return FALSE;
	}

	if (!AttachBrowserListener())
	{
		MessageBox(NULL, "Attaching browser handler failed!", "Error", MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

BOOL Embed::AttachBrowserListener() {
	webHandler = new WebEventHandler();
	HRESULT hr;
	IConnectionPointContainer *icpc;
	IConnectionPoint *icp;
	hr = this->webBrowser2->QueryInterface(IID_IConnectionPointContainer, (void**)&icpc);
	if (hr != S_OK)
		return FALSE;
	hr = icpc->FindConnectionPoint(DIID_DWebBrowserEvents2, &icp);
	if (hr != S_OK)
		return FALSE;
	hr = icp->Advise(webHandler, webHandler->getCookie());
	if (hr != S_OK)
		return FALSE;

	webHandler->setBrowser(this->webBrowser2);
	return TRUE;
}

RECT Embed::PixelToHiMetric(const RECT &_rc) {
	static bool initialized = false;
	static int ppiX, ppiY;
	if (!initialized)
	{
		HDC hdc = ::GetDC(0);
		ppiX = ::GetDeviceCaps(hdc, LOGPIXELSX);
		ppiY = ::GetDeviceCaps(hdc, LOGPIXELSY);
		initialized = true;
	}

	RECT rc;
	rc.left = MulDiv(2540, _rc.left, ppiX);
	rc.top = MulDiv(2540, _rc.top, ppiY);
	rc.right = MulDiv(2540, _rc.right, ppiX);
	rc.bottom = MulDiv(2540, _rc.bottom, ppiY);
	return rc;
}

void Embed::SetRect(const RECT& _rc)
{
	rObj = _rc;
	{
		RECT hiMetricRect = PixelToHiMetric(rObj);
		SIZEL sz;
		sz.cx = hiMetricRect.right - hiMetricRect.left;
		sz.cy = hiMetricRect.bottom - hiMetricRect.top;
		oleObject->SetExtent(DVASPECT_CONTENT, &sz);
	}

	if (oleInPlaceObject != 0)
	{
		oleInPlaceObject->SetObjectRects(&rObj, &rObj);
	}
}

void Embed::Navigate(std::wstring sUrl) {
	bstr_t url(sUrl.c_str());
	variant_t flags(0x02u); //no nav history
	this->webBrowser2->Navigate(url, &flags, 0, 0, 0);
}

//// IUnkown Impl
HRESULT STDMETHODCALLTYPE Embed::QueryInterface(REFIID id, void **obj) {
	if (id == __uuidof(IUnknown))
	{
		(*obj) = static_cast<IOleClientSite*>(this);
	}
	else if (id == __uuidof(IOleInPlaceSite))
	{
		(*obj) = static_cast<IOleInPlaceSite*>(this);
	}
	else 
	{
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

ULONG STDMETHODCALLTYPE Embed::AddRef() {
	refCount++;
	return refCount;
}

ULONG STDMETHODCALLTYPE Embed::Release() {
	refCount--;
	return refCount;
}

//// IOleWindow Impl
HRESULT STDMETHODCALLTYPE Embed::GetWindow(HWND *hwnd) {
	(*hwnd) = pHwnd;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Embed::ContextSensitiveHelp(BOOL enterMode) {
	return E_NOTIMPL;
}

//// IOleInPlaceSite Impl
HRESULT STDMETHODCALLTYPE Embed::CanInPlaceActivate() {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Embed::OnInPlaceActivate() {
	OleLockRunning(oleObject, TRUE, FALSE);
	oleObject->QueryInterface(&oleInPlaceObject);
	oleInPlaceObject->SetObjectRects(&rObj, &rObj);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE Embed::OnUIActivate() {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Embed::OnInPlaceDeactivate() {
	cHwnd = 0;
	oleInPlaceObject = 0;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE Embed::GetWindowContext(IOleInPlaceFrame **frame, IOleInPlaceUIWindow **doc, LPRECT posRect, LPRECT clipRect, LPOLEINPLACEFRAMEINFO frameInfo) {
	HWND hwnd = pHwnd;

	(*frame) = NULL;
	(*doc) = NULL;
	(*posRect).left = rObj.left;
	(*posRect).top = rObj.top;
	(*posRect).right = rObj.right;
	(*posRect).bottom = rObj.bottom;
	*clipRect = *posRect;

	frameInfo->fMDIApp = false;
	frameInfo->hwndFrame = hwnd;
	frameInfo->haccel = NULL;
	frameInfo->cAccelEntries = 0;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE Embed::Scroll(SIZE extant) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::OnUIDeactivate(BOOL undoable) {
	return S_OK;
}

HWND Embed::GetControlWindow() {
	if (cHwnd != 0)
		return cHwnd;

	if (oleInPlaceObject == 0)
		return 0;

	oleInPlaceObject->GetWindow(&cHwnd);
	return cHwnd;
}

HRESULT STDMETHODCALLTYPE Embed::DiscardUndoState() {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::DeactivateAndUndo() {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::OnPosRectChange(LPCRECT posRect) {
	return E_NOTIMPL;
}

//// IOleClientSite Impl
HRESULT STDMETHODCALLTYPE Embed::SaveObject() {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::GetMoniker(DWORD assign, DWORD whichMoniker, IMoniker **mk) {
	if ((assign == OLEGETMONIKER_ONLYIFTHERE) &&
		(whichMoniker == OLEWHICHMK_CONTAINER)) {
		return E_FAIL;
	}

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::GetContainer(IOleContainer **container) {
	return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE Embed::ShowObject() {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Embed::OnShowWindow(BOOL show) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Embed::RequestNewObjectLayout() {
	return E_NOTIMPL;
}

//// IStorage Impl
HRESULT STDMETHODCALLTYPE Embed::CreateStream(const OLECHAR *name, DWORD mode, DWORD r1, DWORD r2, IStream **stm) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::OpenStream(const OLECHAR *name, void *r1, DWORD mode, DWORD r2, IStream **stm) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::CreateStorage(const OLECHAR *name, DWORD mode, DWORD r1, DWORD r2, IStorage **stg) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::OpenStorage(const OLECHAR *name, IStorage *priority, DWORD mode, SNB exclude, DWORD r, IStorage **stg) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::CopyTo(DWORD cExclude, const IID *rExclude, SNB sExclude, IStorage *dest) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::MoveElementTo(const OLECHAR *name, IStorage *dest, const OLECHAR *newName, DWORD flags) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::Commit(DWORD commitFlags) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::Revert() {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::EnumElements(DWORD r1, void* r2, DWORD r3, IEnumSTATSTG **e) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::DestroyElement(const OLECHAR *name) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::RenameElement(const OLECHAR *oName, const OLECHAR *nName) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::SetElementTimes(const OLECHAR *name, const FILETIME *ctime, const FILETIME *atime, const FILETIME *mtime) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::SetClass(REFCLSID id) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Embed::SetStateBits(DWORD stateBits, DWORD mask) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE Embed::Stat(STATSTG *statstg, DWORD statFlag) {
	return E_NOTIMPL;
}

BOOL Embed::SetRegistry() {
	LPCSTR exename = "quark.exe";
	LPCTSTR loc = TEXT("SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION");
	HKEY hKey;

	const BYTE val = 0x2AF9; //IE11 mode

	LONG res = RegOpenKeyEx(HKEY_CURRENT_USER, loc, 0, KEY_SET_VALUE, &hKey);

	if (res != ERROR_SUCCESS) {
		MessageBox(NULL, "Opening registry failed!", "Error", MB_ICONERROR);
		return FALSE;
	}

	res = RegSetValueEx(hKey, exename, 0, REG_DWORD, &val, 1);

	if (res != ERROR_SUCCESS)
		return FALSE;

	res = RegCloseKey(hKey);

	if (res != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}