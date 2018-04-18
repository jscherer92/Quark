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
					events[name].push_back(std::make_pair(std::make_unique<IDispatch>(disp), false));
				}
				else
				{
					//TODO: add in exception info
					return E_FAIL;
				}
			}
			else
			{
				events[name] = std::vector<std::pair<std::unique_ptr<IDispatch>, bool>>();
				events[name].push_back(std::make_pair(std::make_unique<IDispatch>(disp), false));
			}
			return hr;
		}
		case emit: //need to think of a good way of handling this one
		{
			auto numArgs = params->cArgs;
			std::wstring eventName(params->rgvarg[numArgs - 1].bstrVal, SysStringLen(params->rgvarg[numArgs - 1].bstrVal));
			auto val = events.find(eventName);
			if (val != events.end())
			{
				DISPPARAMS p;
				for (unsigned int i = numArgs - 2, j = 0; i >= 0; i--, j++)
				{
					p.rgvarg[j] = params->rgvarg[i];
				}
				p.cArgs = numArgs - 1;
				p.cNamedArgs = 0;
				p.rgdispidNamedArgs = NULL;
				auto vec = val->second;
				auto release = std::vector<int>();
				for (auto i = 0; i < vec.size(); i++)
				{
					VARIANT vResult;
					vec[i].first->Invoke(DISPID_VALUE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &p, &vResult, NULL, NULL);
					if (vec[i].second)
					{
						release.push_back(i);
					}
				}
				//finally, release all of the once listeners
				for (auto i = release.size() - 1; i >= 0; i--)
				{
					vec.erase(vec.begin() + release[i]);
				}
			}
			else
			{
				//TODO: add in exception info
				return E_FAIL;
			}
			return hr;
		}
		case eventNames:
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
			for (auto const& value : names)
			{
				BSTR temp = SysAllocStringLen(value.data(), value.size());
				*pdFreq++ = temp;
			}
			SafeArrayUnaccessData(sf);
			result->parray = sf;
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
			auto temp = params->rgvarg[1].bstrVal;
			std::wstring eventName(temp, SysStringLen(temp));
			result->vt = VT_SAFEARRAY;
			SAFEARRAY* sf;
			SAFEARRAYBOUND rgsabound[1];
			rgsabound[0].lLbound = 0;
			rgsabound[0].cElements = events[eventName].size();
			sf = SafeArrayCreate(VT_DISPATCH, 1, rgsabound);
			IDispatch HUGEP *pdFreq;
			SafeArrayAccessData(sf, (void**)&pdFreq);
			for (auto const& value : events[eventName])
			{
				*pdFreq++ = *value.first.get();
			}
			SafeArrayUnaccessData(sf);
			result->parray = sf;
			return hr;
		}
		case on:
		{
			auto temp = params->rgvarg[1].bstrVal;
			std::wstring name(temp, SysStringLen(temp));
			auto disp = params->rgvarg[0].pdispVal;
			auto val = events.find(name);
			if (val != events.end()) //need to make private function on making sure we are in bounds
			{
				if (events[name].size() < maxListeners)
				{
					events[name].push_back(std::make_pair(std::make_unique<IDispatch>(disp), false));
				}
				else
				{
					//TODO: add in exception info
					return E_FAIL;
				}
			}
			else
			{
				events[name] = std::vector<std::pair<std::unique_ptr<IDispatch>, bool>>();
				events[name].push_back(std::make_pair(std::make_unique<IDispatch>(disp), false));
			}
			return hr;
		}
		case once:
		{
			auto temp = params->rgvarg[1].bstrVal;
			std::wstring name(temp, SysStringLen(temp));
			auto disp = params->rgvarg[0].pdispVal;
			auto val = events.find(name);
			if (val != events.end()) //need to make private function on making sure we are in bounds
			{
				if (events[name].size() < maxListeners)
				{
					events[name].push_back(std::make_pair(std::make_unique<IDispatch>(disp), true));
				}
				else
				{
					//TODO: add in exception info
					return E_FAIL;
				}
			}
			else
			{
				events[name] = std::vector<std::pair< std::unique_ptr<IDispatch>, bool>> ();
				events[name].push_back(std::make_pair(std::make_unique<IDispatch>(disp), true));
			}
			return hr;
		}
		case prependListener:
		{
			auto temp = params->rgvarg[1].bstrVal;
			std::wstring name(temp, SysStringLen(temp));
			auto disp = params->rgvarg[0].pdispVal;
			auto val = events.find(name);
			if (val != events.end()) //need to make private function on making sure we are in bounds
			{
				if (events[name].size() < maxListeners)
				{
					events[name].insert(events[name].begin(), std::make_pair(std::make_unique<IDispatch>(disp), false));
				}
				else
				{
					//TODO: add in exception info
					return E_FAIL;
				}
			}
			else
			{
				events[name] = std::vector<std::pair< std::unique_ptr<IDispatch>, bool>>();
				events[name].insert(events[name].begin(), std::make_pair(std::make_unique<IDispatch>(disp), false));
			}
			return hr;
		}
		case prependOnceListener:
		{
			auto temp = params->rgvarg[1].bstrVal;
			std::wstring name(temp, SysStringLen(temp));
			auto disp = params->rgvarg[0].pdispVal;
			auto val = events.find(name);
			if (val != events.end()) //need to make private function on making sure we are in bounds
			{
				if (events[name].size() < maxListeners)
				{
					events[name].insert(events[name].begin(), std::make_pair(std::make_unique<IDispatch>(disp), true));
				}
				else
				{
					//TODO: add in exception info
					return E_FAIL;
				}
			}
			else
			{
				events[name] = std::vector<std::pair< std::unique_ptr<IDispatch>, bool>>();
				events[name].insert(events[name].begin(), std::make_pair(std::make_unique<IDispatch>(disp), true));
			}
			return hr;
		}
		case removeAllListeners:
		{
			auto numArgs = params->cArgs;
			if (numArgs == 0) //delete all listeners
			{
				events.clear();
			}
			else
			{
				std::wstring eventName(params->rgvarg[0].bstrVal, SysStringLen(params->rgvarg[0].bstrVal));
				auto val = events.find(eventName);
				if (val != events.end())
				{
					events.erase(val);
				}
				result->pdispVal = this;
				result->vt = VT_DISPATCH;
			}
			return hr;
		}
		case removeListener: //will have to think about this one, but I don't think it is readily needed
		{
			return E_NOTIMPL;
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