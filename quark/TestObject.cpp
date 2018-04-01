#include "TestObject.h"

HRESULT TestObject::getMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) {
	//if we are greater than our location and less then the next person's location
	if (id > locs[GET] && id < locs[SET]) {
		//run the method we want
		auto loc = id - locs[GET];
		switch (loc) {
		case 1: //GetValue method, should probably enum this also
			result->vt = VT_BSTR;
			result->bstrVal = testVal;
			break;
		default:
			return  DISP_E_MEMBERNOTFOUND;
		}
		return S_OK;
	}

	return E_FAIL;
}

HRESULT TestObject::setMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) {
	if (id > locs[SET] && id < locs[CREATE]) {
		//run the corresponding method
		auto loc = id - locs[SET];
		switch(loc) {
		case 1:
			if (params->cArgs >= 1 &&
				params->rgvarg[0].vt == VT_BSTR) {
				testVal = params->rgvarg[0].bstrVal;
			}
			break;
		default:
			return DISP_E_MEMBERNOTFOUND;
			break;
		}
		return S_OK;
	}

	return E_FAIL;
}

HRESULT TestObject::createObject(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) {
	if (id > locs[CREATE] && id < locs[CALL]) {
		return S_OK;
	}

	return E_FAIL;
}

HRESULT TestObject::callFunction(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) {
	if (id > locs[CALL] && id <= mMems.size()) {
		auto loc = id - locs[CALL];
		switch (loc) {
		case 1: {
			result->vt = VT_BSTR;
			result->bstrVal = testVal;
			break;
		}
		case 2: {
			if (params->cArgs >= 1 &&
				params->rgvarg[0].vt == VT_BSTR) {
				testVal = params->rgvarg[0].bstrVal;
			}
			break;
		}
		case 3: {
			HRESULT res = S_OK;
			LONG numArgs = params->cArgs;
			BSTR name = nullptr;
			//by getting the VT_BSTR we can switch on this and figure out the event that was fired from this!
			if (params->rgvarg[1].vt == VT_BSTR) {
				name = params->rgvarg[1].bstrVal;
			}
			if (0 == wcscmp(name, L"addNewVal")) {
				if (params->rgvarg[0].vt == VT_DISPATCH) {
					std::unique_ptr<DISPPARAMS> p = std::make_unique<DISPPARAMS>();
					std::unique_ptr<VARIANT[]> args = std::make_unique<VARIANT[]>(1);
					args[0].vt = VT_BSTR;
					args[0].bstrVal = SysAllocString(L"This came from C=+ and passed as a param to the callback!");
					p->rgvarg = args.release();
					p->rgdispidNamedArgs = nullptr;
					p->cArgs = 1;
					p->cNamedArgs = 0;
					params->rgvarg[0].pdispVal->Invoke(DISPID_VALUE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, p.release(), NULL, NULL, NULL);
				}
			}
			if (name != nullptr)
				SysFreeString(name);
			break;
		}
		default: {
			return DISP_E_MEMBERNOTFOUND;
			break;
		}
		}
		//we need to release the unique ptr back to the invoke function, otherwise unique_ptr will clean up
		//and destroy the objects that we have created
		params.release();
		result.release();
		exception.release();
		errArg.release();
		return S_OK;
	}

	return E_FAIL;
}