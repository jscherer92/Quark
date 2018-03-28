#ifndef test_HEADER
#define test_HEADER

#include "Test.h"
#include <iostream>
#include <string>

HRESULT STDMETHODCALLTYPE Test::QueryInterface(REFIID id, void **v) {
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

ULONG STDMETHODCALLTYPE Test::AddRef() {
	return InterlockedIncrement(&ref);
}

ULONG STDMETHODCALLTYPE Test::Release() {
	int t = InterlockedIncrement(&ref);

	if (t == 0)
	{
		delete this;
	}

	return t;
}

HRESULT STDMETHODCALLTYPE Test::GetTypeInfoCount(UINT *info) {
	*info = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Test::GetTypeInfo(UINT tInfo, LCID cid, ITypeInfo **pTInfo) {
	return E_FAIL;
}

HRESULT STDMETHODCALLTYPE Test::GetIDsOfNames(REFIID id, LPOLESTR *names, UINT cNames, LCID cid, DISPID *dispId) {
	HRESULT hr = S_OK;

	for (UINT i = 0; i < cNames; i++) {
		auto it = std::distance(methods.begin(), std::find(methods.begin(), methods.end(), std::wstring(names[i])));
		if (it != methods.size()) {
			dispId[i] = DISPID_VALUE + it + 1;
		}
		else {
			dispId[i] = DISPID_UNKNOWN;
			hr = DISP_E_UNKNOWNNAME;
		}
	}

	return hr;
}

HRESULT STDMETHODCALLTYPE Test::Invoke(DISPID dispId, REFIID id, LCID cId, WORD flags, DISPPARAMS *dispParams, VARIANT *result, EXCEPINFO *excepInfo, UINT *argErr) {

	if (flags & (DISPATCH_METHOD | DISPATCH_PROPERTYGET | DISPATCH_PROPERTYPUT))
	{

		HRESULT hr = S_OK;

		switch (dispId)
		{
		case DISP_SETVAL: {
			//we have values, but we only care about the first one and that it is a string
			if (dispParams->cArgs >= 1 &&
				dispParams->rgvarg[0].vt == VT_BSTR) {
				testVal = dispParams->rgvarg[0].bstrVal;
			}
			break;
		}
		case DISP_GETVAL: {
			result->vt = VT_BSTR;
			result->bstrVal = testVal;
			break;
		}
		case DISP_ON: {
			HRESULT res = S_OK;
			LONG numArgs = dispParams->cArgs;
			BSTR name = nullptr;
			//by getting the VT_BSTR we can switch on this and figure out the event that was fired from this!
			if (dispParams->rgvarg[1].vt == VT_BSTR) {
				name = dispParams->rgvarg[1].bstrVal;
			}
			if (0 == wcscmp(name, L"addNewVal")) {
				if (dispParams->rgvarg[0].vt == VT_DISPATCH) {
					std::unique_ptr<DISPPARAMS> params(new DISPPARAMS);
					std::unique_ptr<VARIANT[]> args(new VARIANT[1]);
					args[0].vt = VT_BSTR;
					args[0].bstrVal = SysAllocString(L"This came from C=+ and passed as a param to the callback!");
					params->rgvarg = args.release();
					params->rgdispidNamedArgs = nullptr;
					params->cArgs = 1;
					params->cNamedArgs = 0;
					dispParams->rgvarg[0].pdispVal->Invoke(DISPID_VALUE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, params.release(), NULL, NULL, NULL);
				}
			}
			if (name != nullptr)
				SysFreeString(name);
			break;
		}

		default: {
			hr = DISP_E_MEMBERNOTFOUND;
		}

		}

		return hr;
	}

	return E_FAIL;
}
#endif