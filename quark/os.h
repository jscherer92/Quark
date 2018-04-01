#include <Windows.h>
#include <DispEx.h>
#include <vector>
#include <algorithm>
#include <memory>
#include "TestObject.h"

class OpSys : public IDispatch {
private:
	std::vector<std::wstring> methods;
	static const DISPID DISP_EOL = DISPID_VALUE + 1;
	static const DISPID DISP_ARCH = DISPID_VALUE + 2;
	static const DISPID DISP_CONST = DISPID_VALUE + 3;
	static const DISPID DISP_CPUS = DISPID_VALUE + 4;
	static const DISPID DISP_ENDIAN = DISPID_VALUE + 5;
	static const DISPID DISP_FMEM = DISPID_VALUE + 6;
	static const DISPID DISP_HOMDIR = DISPID_VALUE + 7;
	static const DISPID DISP_OBJTEST = DISPID_VALUE + 8;
	static const DISPID DISP_ON = DISPID_VALUE + 9;
	long ref;
public:
	OpSys() {
		methods = { L"EOL", L"arch", L"constants", L"cpus", L"endianness", L"freemem", L"homedir", L"Test", L"on" };
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

class TestObj {
private:
	int member;
public:
	TestObj(int m) : member(m) {};
	int getMember() {
		return member;
	};
	void setMember(int value) {
		member = value;
	}
};