#include <Windows.h>
#include <exdisp.h>
#include <ExDispid.h>
#include <DispEx.h>
#include <MsHTML.h>
#include <string>
#include <comdef.h>
#include "os.h"

class WebEventHandler : public IDispatch {
public:
	WebEventHandler() {
		cookie = new DWORD;
	};

	DWORD* getCookie() {
		return cookie;
	}
	void setBrowser(IWebBrowser2 *pWeb) {
		wb = pWeb;
	}
	BOOL AttachScriptHandler();

	// IUnkown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID id, void **v);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	// IDispatch
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo);
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT tInfo, LCID id, ITypeInfo **pTInfo);
	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID id, LPOLESTR *names, UINT cNames, LCID cId, DISPID *dispId);
	virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID idMember, REFIID id, LCID cId, WORD flags, DISPPARAMS *dispParams, VARIANT *result, EXCEPINFO *excepInfo, UINT *argErr);
private:
	long ref;
	DWORD *cookie;
	IWebBrowser2 *wb;
	OpSys *Os;
};