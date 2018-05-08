#include "WriteableStream.h"

//we are going to attach the close, drain, error, finish, pipe, and unpipe events
void WriteableStream::attachEvents() {
	events[std::wstring(L"close")] = std::vector<std::pair<std::unique_ptr<IDispatch>, bool>>();
	events[std::wstring(L"drain")] = std::vector<std::pair<std::unique_ptr<IDispatch>, bool>>();
	events[std::wstring(L"error")] = std::vector<std::pair<std::unique_ptr<IDispatch>, bool>>();
	events[std::wstring(L"finish")] = std::vector<std::pair<std::unique_ptr<IDispatch>, bool>>();
	events[std::wstring(L"pipe")] = std::vector<std::pair<std::unique_ptr<IDispatch>, bool>>();
	events[std::wstring(L"unpipe")] = std::vector<std::pair<std::unique_ptr<IDispatch>, bool>>();
}

HRESULT WriteableStream::callFunction(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) {
	HRESULT hr = S_OK;
	if (id > locs[CALL] && id <= mMems.size()) {
		auto loc = id - locs[CALL];
		switch (loc)
		{
		case cork: {
			corked = true;
			break;
		}
		case uncork: {
			if (ibuf.size())
			{
				for (auto i : ibuf) {
					outStream.put(i);
				}
				ibuf.clear();
			}
			corked = false;
		}
		}
	} 
	else
	{
		EventEmitter::callFunction(id, std::move(params), std::move(result), std::move(exception), std::move(errArg));
	}
}