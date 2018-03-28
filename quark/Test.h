#include <Windows.h>
#include <DispEx.h>
#include <vector>
#include <algorithm>
#include <memory>

class Test : public IDispatch {
private:
	std::vector<std::wstring> methods;
	static const DISPID DISP_GETVAL = DISPID_VALUE + 1;
	static const DISPID DISP_SETVAL = DISPID_VALUE + 2;
	static const DISPID DISP_ON = DISPID_VALUE + 3;
	long ref;
	BSTR testVal;
public:
	Test() {
		testVal = SysAllocString(L"Test");
		methods = { L"GetValue", L"SetValue", L"on" };
	};

	// IUnkown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID id, void **v);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	// IDispatch
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo);
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT tInfo, LCID id, ITypeInfo **pTInfo);
	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID id, LPOLESTR *names, UINT cNames, LCID cId, DISPID *dispId);
	virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID idMember, REFIID id, LCID cId, WORD flags, DISPPARAMS *dispParams, VARIANT *result, EXCEPINFO *excepInfo, UINT *argErr);
};