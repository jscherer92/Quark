#ifndef os_HEADER
#define os_HEADER

#include "os.h"

HRESULT STDMETHODCALLTYPE OpSys::QueryInterface(REFIID id, void **v) {
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

ULONG STDMETHODCALLTYPE OpSys::AddRef() {
	return InterlockedIncrement(&ref);
}

ULONG STDMETHODCALLTYPE OpSys::Release() {
	int t = InterlockedIncrement(&ref);

	if (t == 0)
	{
		delete this;
	}

	return t;
}

HRESULT STDMETHODCALLTYPE OpSys::GetTypeInfoCount(UINT *info) {
	*info = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE OpSys::GetTypeInfo(UINT tInfo, LCID cid, ITypeInfo **pTInfo) {
	return E_FAIL;
}

HRESULT STDMETHODCALLTYPE OpSys::GetIDsOfNames(REFIID id, LPOLESTR *names, UINT cNames, LCID cid, DISPID *dispId) {
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

HRESULT STDMETHODCALLTYPE OpSys::Invoke(DISPID dispId, REFIID id, LCID cId, WORD flags, DISPPARAMS *dispParams, VARIANT *result, EXCEPINFO *excepInfo, UINT *argErr) {
	if (flags & DISPATCH_METHOD)
	{
		HRESULT hr = S_OK;

		/* We will implement this loop later, atm the methods we are creating do not use args at all
		auto args = std::make_unique<std::string[]>(dispParams->cArgs);
		for (auto i = 0; i < dispParams->cArgs; ++i) {
			BSTR arg = dispParams->rgvarg[i].
		}
		*/

		switch (dispId)
		{
		case DISP_ARCH: {
			auto ret = SysAllocString(L"Called for ARCH");
			if (result != nullptr)
			{
				result->vt = VT_BSTR;
				result->bstrVal = ret;
			}
			break;
		}
		case DISP_CONST: {
			auto ret = SysAllocString(L"Called for constants");
			result->vt = VT_BSTR;
			result->bstrVal = ret;
			break;
		}
		case DISP_CPUS: {
			auto ret = SysAllocString(L"Called for number of cpus");
			result->vt = VT_BSTR;
			result->bstrVal = ret;
			break;
		}
		case DISP_ENDIAN: {
			auto ret = SysAllocString(L"Called for Endianess of system");
			result->vt = VT_BSTR;
			result->bstrVal = ret;
			break;
		}
		case DISP_EOL: {
			auto ret = SysAllocString(L"Called for EOL qualifier");
			result->vt = VT_BSTR;
			result->bstrVal = ret;
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