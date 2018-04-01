#include "Buffer.h";

HRESULT Buffer::getMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) 
{
	HRESULT hr = S_OK;
	if (id > locs[GET] && id <= locs[SET]) 
	{
		auto loc = id - locs[GET];
		switch (loc) {
		case poolSize:
		{
			break;
		}
		case buffer:
		{
			break;
		}
		case length:
		{
			break;
		}
		case INSPECT_MAX_BYTES:
		{
			break;
		}
		case kMaxLength:
		{
			break;
		}
		}
	}
	return E_NOINTERFACE;
};
HRESULT Buffer::setMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg)
{
	return E_NOINTERFACE;
};
//virtual HRESULT setReferenceMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) = 0;
HRESULT Buffer::createObject(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg)
{
	return E_NOINTERFACE;
};
HRESULT Buffer::callFunction(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg)
{
	HRESULT hr = S_OK;
	if (id > locs[CALL] && id <= mMems.size())
	{
		auto loc = id - locs[CALL];
		switch (loc)
		{
		case alloc:
		{
			break;
		}
		case allocUnsafe:
		{
			break;
		}
		case allocUnsafeSlow:
		{
			break;
		}
		case byteLength:
		{
			break;
		}
		case compare:
		{
			break;
		}
		case concat:
		{
			break;
		}
		case from:
		{
			break;
		}
		case isBuffer:
		{
			break;
		}
		case isEncoding:
		{
			break;
		}
		case copy:
		{
			break;
		}
		case entries:
		{
			break;
		}
		case equals:
		{
			break;
		}
		case fill:
		{
			break;
		}
		case includes:
		{
			break;
		}
		case indexOf:
		{
			break;
		}
		case keys:
		{
			break;
		}
		case lastIndexOf:
		{
			break;
		}
		case readDoubleBE:
		{
			break;
		}
		case readDoubleLE:
		{
			break;
		}
		case readFloatBE:
		{
			break;
		}
		case readFloatLE:
		{
			break;
		}
		case readInt8:
		{
			break;
		}
		case readInt16BE:
		{
			break;
		}
		case readInt16LE:
		{
			break;
		}
		case readInt32BE:
		{
			break;
		}
		case readInt32LE:
		{
			break;
		}
		case readIntBE:
		{
			break;
		}
		case readIntLE:
		{
			break;
		}
		case slice:
		{
			break;
		}
		case swap16:
		{
			break;
		}
		case swap32:
		{
			break;
		}
		case swap64:
		{
			break;
		}
		case toJSON:
		{
			break;
		}
		case toString:
		{
			break;
		}
		case values:
		{
			break;
		}
		case write:
		{
			break;
		}
		case writeDoubleBE:
		{
			break;
		}
		case writeDoubleLE:
		{
			break;
		}
		case writeFloatBE:
		{
			break;
		}
		case writeFloatLE:
		{
			break;
		}
		case writeInt8:
		{
			break;
		}
		case writeInt16BE:
		{
			break;
		}
		case writeInt16LE:
		{
			break;
		}
		case writeInt32BE:
		{
			break;
		}
		case writeInt32LE:
		{
			break;
		}
		case writeIntBE:
		{
			break;
		}
		case writeIntLE:
		{
			break;
		}
		case writeUInt8:
		{
			break;
		}
		case writeUInt16BE:
		{
			break;
		}
		case writeUInt16LE:
		{
			break;
		}
		case writeUInt32BE:
		{
			break;
		}
		case writeUInt32LE:
		{
			break;
		}
		case writeUIntBE:
		{
			break;
		}
		case writeUIntLE:
		{
			break;
		}
		case transcode:
		{
			break;
		}
		}
	}
	return E_NOINTERFACE;
};