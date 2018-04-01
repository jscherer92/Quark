#include "BaseObject.h"
class TestObject : public BaseObject {
private:
	virtual HRESULT getMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;
	virtual HRESULT setMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;
	//virtual HRESULT setReferenceMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) = 0;
	virtual HRESULT createObject(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;
	virtual HRESULT callFunction(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;
	BSTR testVal;
public:
	TestObject() : BaseObject({
		{SET, {}},
		{GET, {}},
		{CREATE, {}},
		{CALL, {L"GetValue", L"SetValue", L"on"}}
	}) {
		testVal = SysAllocString(L"TestVal");
	};
};