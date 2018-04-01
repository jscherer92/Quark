#include "TestObject.h"

HRESULT TestObject::getMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) {
	//if we are greater than our location and less then the next person's location
	if (id > locs[GET] && id < locs[SET]) {
		//run the method we want

		return S_OK;
	}

	return E_FAIL;
}

HRESULT TestObject::setMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) {
	if (id > locs[SET] && id < locs[CREATE]) {
		//run the corresponding method

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
	if (id > locs[CALL] && id < mMems.size()) {
		return S_OK;
	}

	return E_FAIL;
}