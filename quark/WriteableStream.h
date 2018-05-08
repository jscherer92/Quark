#include <ostream>
#include "EventEmitter.h"
#include "Buffer.h"

class WriteableStream : public EventEmitter {
protected:
	virtual HRESULT getMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;
	virtual HRESULT setMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;
	//virtual HRESULT setReferenceMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) = 0;
	virtual HRESULT createObject(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;
	virtual HRESULT callFunction(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;

	enum getMethods {
		writableHighWaterMark = 1
	};

	enum callMethods {
		cork	 = 1,
		end		 = 2,
		setDefaultEncoding = 3,
		uncork   = 4,
		write    = 5,
		destroy  = 6,
		_write   = 7,
		_writev  = 8,
		_destroy = 9,
		_final   = 10
	};

	std::map<memberType, std::vector<std::wstring>> methodMap = {
		{ SET,{} },
		{ GET,{ L"writableHighWaterMark" } },
		{ CREATE,{} },
		{ CALL,{
			L"cork",
			L"end",
			L"setDefaultEncoding",
			L"uncork",
			L"write",
			L"destroy",
			L"_write",
			L"_writev",
			L"_destroy",
			L"_final"
		} }
	};
public:
	WriteableStream() : EventEmitter(methodMap) {}
private:
	void attachEvents();
	std::ostream outStream;
	std::vector<byte> ibuf;
	bool corked = false;
};