#include "WriteableStream.h"

HRESULT WriteableStream::callFunction(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) {
	HRESULT hr = S_OK;
	if (id > locs[CALL] && id <= mMems.size()) {
	
	} 
	else
	{
		EventEmitter::callFunction(id, std::move(params), std::move(result), std::move(exception), std::move(errArg));
	}
}