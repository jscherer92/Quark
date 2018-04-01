#include <Windows.h>
#include <DispEx.h>
#include <vector>
#include <algorithm>
#include <memory>
#include <map>

class BaseObject : public IDispatch {
private:
	long ref;

	// methods to overwrite by deriving this class
	virtual HRESULT getMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) = 0;
	virtual HRESULT setMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) = 0;
	//virtual HRESULT setReferenceMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) = 0;
	virtual HRESULT createObject(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) = 0;
	virtual HRESULT callFunction(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) = 0;
protected:
	const enum memberType { GET = 0, SET = 1, CREATE = 2, CALL = 3 };
	const std::map<memberType, std::vector<std::wstring>> members;
	std::vector<std::wstring> mMems;
	byte locs[4];
	byte size[4];
	const WORD allowedFlags = DISPATCH_METHOD | DISPATCH_PROPERTYGET | DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF | DISPATCH_CONSTRUCT;
public:
	BaseObject(std::map<memberType, std::vector<std::wstring>>& mems)
	{
		auto it = 1;
		locs[0] = 0;
		for (auto& el : mems) {
			size[it - 1] = el.second.size();
			mMems.insert(std::end(mMems), std::begin(el.second), std::end(el.second));
			locs[it] = mMems.size() - 1;
		}
	}

	BaseObject(std::map<memberType, std::vector<std::wstring>>&& mems)
	{
		auto it = 1;
		locs[0] = 0;
		for (auto&& el : mems) {
			size[it - 1] = el.second.size();
			mMems.insert(std::end(mMems), std::begin(el.second), std::end(el.second));
			locs[it] = mMems.size() - 1;
		}
	};
	// IUnkown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID id, void **v);
	virtual ULONG   STDMETHODCALLTYPE AddRef();
	virtual ULONG   STDMETHODCALLTYPE Release();

	// IDispatch
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo);
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT tInfo, LCID id, ITypeInfo **pTInfo);
	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID id, LPOLESTR *names, UINT cNames, LCID cId, DISPID *dispId);
	virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID idMember, REFIID id, LCID cId, WORD flags, DISPPARAMS *dispParams, VARIANT *result, EXCEPINFO *excepInfo, UINT *argErr);
};