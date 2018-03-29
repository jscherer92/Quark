#include "webEventHandler.h"

BOOL WebEventHandler::AttachScriptHandler() {
	HRESULT hr;
	IDispatch *docDispatch;
	IHTMLDocument2 *doc;
	IHTMLWindow2 *win;
	IDispatchEx *wndEx;
	DISPID dispid;
	hr = wb->get_Document(&docDispatch);
	if (hr != S_OK)
		return FALSE;
	hr = docDispatch->QueryInterface(&doc);
	if (hr != S_OK)
		return FALSE;
	docDispatch->Release();
	hr = doc->get_parentWindow(&win);
	if (hr != S_OK)
		return FALSE;
	doc->Release();
	hr = win->QueryInterface(&wndEx);
	if (hr != S_OK)
		return FALSE;
	win->Release();
	BSTR objName = SysAllocString(L"os");
	wndEx->GetDispID(objName, fdexNameEnsure, &dispid);
	SysFreeString(objName);
	Os = new OpSys();
	Os->AddRef();

	DISPID namedArgs[] = { DISPID_PROPERTYPUT };
	DISPPARAMS params;
	params.rgvarg = new VARIANT[1];
	params.rgvarg[0].pdispVal = Os;
	params.rgvarg[0].vt = VT_DISPATCH;
	params.rgdispidNamedArgs = namedArgs;
	params.cArgs = 1;
	params.cNamedArgs = 1;
	wndEx->InvokeEx(dispid, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, NULL, NULL, NULL);
	wndEx->Release();

	return hr;
}

HRESULT STDMETHODCALLTYPE WebEventHandler::QueryInterface(REFIID id, void **v) {
	*v = nullptr;
	if (id == IID_IUnknown || id == IID_IDispatch)
	{
		*v = static_cast<IDispatch*>(this);
	}

	if (*v != nullptr)
	{
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE WebEventHandler::AddRef() {
	return InterlockedIncrement(&ref);
}

ULONG STDMETHODCALLTYPE WebEventHandler::Release() {
	int t = InterlockedIncrement(&ref);

	if (t == 0)
	{
		delete this;
	}

	return t;
}

HRESULT STDMETHODCALLTYPE WebEventHandler::GetTypeInfoCount(UINT *info) {
	*info = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE WebEventHandler::GetTypeInfo(UINT tInfo, LCID cid, ITypeInfo **pTInfo) {
	return E_FAIL;
}

HRESULT STDMETHODCALLTYPE WebEventHandler::GetIDsOfNames(REFIID id, LPOLESTR *names, UINT cNames, LCID cid, DISPID *dispId) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE WebEventHandler::Invoke(DISPID dispId, REFIID id, LCID cId, WORD flags, DISPPARAMS *dispParams, VARIANT *result, EXCEPINFO *excepInfo, UINT *argErr) {
	if (flags & DISPATCH_METHOD)
	{
		HRESULT hr = S_OK;

		switch (dispId)
		{
		case DISPID_NAVIGATECOMPLETE2:
		{
			AttachScriptHandler();
			break;
		}
		default:
		{
			hr = DISP_E_MEMBERNOTFOUND;
		}
		}

		return hr;
		
	}
	return E_FAIL;
}