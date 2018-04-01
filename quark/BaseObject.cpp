#ifndef baseobj_HEADER
#define baseobj_HEADER

#include "BaseObject.h"
#include <iostream>
#include <string>

HRESULT STDMETHODCALLTYPE BaseObject::QueryInterface(REFIID id, void **v) {
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

ULONG STDMETHODCALLTYPE BaseObject::AddRef() {
	return InterlockedIncrement(&ref);
}

ULONG STDMETHODCALLTYPE BaseObject::Release() {
	int t = InterlockedIncrement(&ref);

	if (t == 0)
	{
		delete this;
	}

	return t;
}

HRESULT STDMETHODCALLTYPE BaseObject::GetTypeInfoCount(UINT *info) {
	*info = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE BaseObject::GetTypeInfo(UINT tInfo, LCID cid, ITypeInfo **pTInfo) {
	return E_FAIL;
}

HRESULT STDMETHODCALLTYPE BaseObject::GetIDsOfNames(REFIID id, LPOLESTR *names, UINT cNames, LCID cid, DISPID *dispId) {
	HRESULT hr = S_OK;

	for (UINT i = 0; i < cNames; i++) {
		auto it = std::distance(mMems.begin(), std::find(mMems.begin(), mMems.end(), std::wstring(names[i])));
		if (it != mMems.size()) {
			dispId[i] = DISPID_VALUE + it + 1;
		}
		else {
			dispId[i] = DISPID_UNKNOWN;
			hr = DISP_E_UNKNOWNNAME;
		}
	}

	return hr;
}

HRESULT STDMETHODCALLTYPE BaseObject::Invoke(DISPID dispId, REFIID id, LCID cId, WORD flags, DISPPARAMS *dispParams, VARIANT *result, EXCEPINFO *excepInfo, UINT *argErr) {

	HRESULT hr = S_OK;

	switch (flags)
	{
	case DISPATCH_METHOD:
	{
		hr = callFunction(dispId,
			std::move(std::unique_ptr<DISPPARAMS>(dispParams)),
			std::move(std::unique_ptr<VARIANT>(result)), 
			std::move(std::unique_ptr<EXCEPINFO>(excepInfo)), 
			std::move(std::unique_ptr<UINT>(argErr)));
		break;
	}
	case DISPATCH_PROPERTYGET:
	{
		hr = getMember(dispId,
			std::move(std::unique_ptr<DISPPARAMS>(dispParams)),
			std::move(std::unique_ptr<VARIANT>(result)),
			std::move(std::unique_ptr<EXCEPINFO>(excepInfo)),
			std::move(std::unique_ptr<UINT>(argErr)));
		break;
	}
	case DISPATCH_PROPERTYPUT:
	{
		hr = setMember(dispId,
			std::move(std::unique_ptr<DISPPARAMS>(dispParams)),
			std::move(std::unique_ptr<EXCEPINFO>(excepInfo)),
			std::move(std::unique_ptr<UINT>(argErr)));
		break;
	}
	case DISPATCH_CONSTRUCT:
	{
		hr = createObject(dispId,
			std::move(std::unique_ptr<DISPPARAMS>(dispParams)),
			std::move(std::unique_ptr<VARIANT>(result)),
			std::move(std::unique_ptr<EXCEPINFO>(excepInfo)),
			std::move(std::unique_ptr<UINT>(argErr)));
		break;
	}
	default:
		hr = DISP_E_MEMBERNOTFOUND;
		break;
	}

	
	return hr;
}
#endif