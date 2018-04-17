#include "EventEmitter.h"

HRESULT EventEmitter::setMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) {
	return E_NOINTERFACE;
}

HRESULT EventEmitter::createObject(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) {
	return E_NOINTERFACE;
}

HRESULT EventEmitter::getMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) {
	HRESULT hr = S_OK;
	if (id > locs[GET] && id <= locs[SET]) {
		auto loc = id - locs[GET];
		switch (loc) 
		{
		case defaultMaxListeners:
		{
			result->lVal = defaultMaxListeners;
			result->vt = VT_I8;
			return hr;
		}
		}
	}
	return E_NOINTERFACE;
}

HRESULT EventEmitter::callFunction(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) {
	HRESULT hr = S_OK;
	if (id > locs[CALL] && id <= mMems.size())
	{
		auto loc = id - locs[CALL];
		switch (loc)
		{
		case addListener:
		{
			auto temp = params->rgvarg[1].bstrVal;
			std::wstring name(temp, SysStringLen(temp));
			auto disp = params->rgvarg[0].pdispVal;
			auto val = events.find(name);
			if (val != events.end()) //need to make private function on making sure we are in bounds
			{
				if (events[name].size() < maxListeners)
				{
					events[name].push(std::make_unique<IDispatch>(disp));
				}
				else
				{
					//TODO: add in exception info
					return S_FALSE;
				}
			}
			else
			{
				events[name] = std::queue<std::unique_ptr<IDispatch>>();
				events[name].push(std::make_unique<IDispatch>());
			}
			return hr;
		}
		case emit: //need to think of a good way of handling this one
		{
			std::vector<std::wstring> names;
			for (auto it = events.begin(); it != events.end(); ++it)
			{
				names.push_back(it->first);
			}
			result->vt = VT_SAFEARRAY;
			SAFEARRAY* sf;
			SAFEARRAYBOUND rgsabound[1];
			rgsabound[0].lLbound = 0;
			rgsabound[0].cElements = names.size();
			sf = SafeArrayCreate(VT_BSTR, 1, rgsabound);
			BSTR HUGEP *pdFreq;
			SafeArrayAccessData(sf, (void**)&pdFreq);
			for (auto const& value: names)
			{
				BSTR temp = SysAllocStringLen(value.data(), value.size());
				*pdFreq++ = temp;
			}
			SafeArrayUnaccessData(sf);
			result->parray = sf;
			return hr;
		}
		case eventNames:
		{
			return hr;
		}
		case getMaxListeners:
		{
			long l;
			l = (maxListeners == -1) ? defaultMaxListeners : maxListeners;
			result->vt = VT_I8;
			result->lVal = l;
			return hr;
		}
		case listenerCount:
		{
			auto temp = params->rgvarg[1].bstrVal;
			std::wstring name(temp, SysStringLen(temp));
			auto disp = params->rgvarg[0].pdispVal;
			auto val = events.find(name);
			result->vt = VT_I8;
			if (val == events.end())
			{
				result->lVal = 0;
			}
			else
			{
				result->lVal = events[name].size();
			}
			return hr;
		}
		case listeners:
		{
			return hr;
		}
		case on:
		{
			return hr;
		}
		case once:
		{
			return hr;
		}
		case prependListener:
		{
			return hr;
		}
		case prependOnceListener:
		{
			return hr;
		}
		case removeAllListeners:
		{
			return hr;
		}
		case removeListener:
		{
			return hr;
		}
		case setMaxListeners: //only handle the 0 case for infinity, not the infinite set case. Will look into later
		{
			auto numArgs = params->cArgs;
			auto num = params->rgvarg[0].lVal;
			if (num < 0)
			{
				BSTR source = SysAllocString(L"Quark.exe");
				BSTR desc = SysAllocString(L"Illegal Argument! Number must be a non negative number!");
				exception->wCode = 1001;
				exception->wReserved = 0;
				exception->bstrSource = source;
				exception->bstrDescription = desc;
				exception->bstrHelpFile = NULL;
				exception->dwHelpContext = NULL;
				exception->pvReserved = NULL;
				exception->pfnDeferredFillIn = NULL;
				exception->scode = 0;
				SysFreeString(source);
				SysFreeString(desc);
			}
			else
			{
				maxListeners = num;
				result->vt = VT_DISPATCH;
				result->pdispVal = this;
			}
			return hr;
		}
		}
	}
	return E_NOINTERFACE;
}