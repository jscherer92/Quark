#include "BaseObject.h"
#include <queue>
#include <utility>

class EventEmitter : public BaseObject {
private:
	long defaultMaxListeners = 10;
	long maxListeners = -1;
	std::map<std::wstring, std::vector<std::pair<std::unique_ptr<IDispatch>, bool>>> events;

	virtual HRESULT getMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;
	virtual HRESULT setMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;
	//virtual HRESULT setReferenceMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) = 0;
	virtual HRESULT createObject(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;
	virtual HRESULT callFunction(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;

	enum getMethods { defaultMaxListeners = 1 };

	enum callMethods {
		addListener = 1,
		emit = 2,
		eventNames = 3,
		getMaxListeners = 4,
		listenerCount = 5,
		listeners = 6,
		on = 7,
		once = 8,
		prependListener = 9,
		prependOnceListener = 10,
		removeAllListeners = 11,
		removeListener = 12,
		setMaxListeners = 13
	};
	std::map<memberType, std::vector<std::wstring>> methodMap = {
		{ SET,{} },
		{ GET,{ L"defaultMaxListeners" } },
		{ CREATE,{} },
		{ CALL,{
			L"addListener",
			L"emit",
			L"eventNames",
			L"getMaxListeners",
			L"listenerCount",
			L"listeners",
			L"on",
			L"once",
			L"prependListener",
			L"prependOnceListener",
			L"removeAllListeners",
			L"removeListener",
			L"setMaxListeners"
		} }
	};
public:
	EventEmitter() : BaseObject(methodMap) { }
};