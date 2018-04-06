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
			result->vt = VT_DISPATCH;
			result->pdispVal = internArr.get();
			break;
		}
		case length:
		{
			result->vt = VT_INT;
			result->intVal = sizeof(internArr->getBase());
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
//right now, implement inline where possible. Optimizations can be made afterward
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
			auto numArgs = params->cArgs;
			if (!numArgs) 
			{
				result->vt = VT_DISPATCH;
				result->pdispVal = (std::make_unique<Buffer>()).release();
			}
			else if (numArgs == 1) { //length argument, no error checking. Need to add that later!!!
				auto len = params->rgvarg[0].intVal;
				result->vt = VT_DISPATCH;
				result->pdispVal = (std::make_unique<Buffer>(true, len)).release();
			}
			else if (numArgs == 2) //length and fill
			{ 
				auto len = params->rgvarg[0].intVal;
				auto data = params->rgvarg[1].bstrVal;
				int bstrLen = WideCharToMultiByte(CP_UTF8, 0, data, -1, NULL, 0, NULL, NULL);
				if (bstrLen)
				{
					auto fData = std::make_unique<byte[]>(bstrLen);
					WideCharToMultiByte(CP_UTF8, 0, data, -1, (LPSTR)fData.get(), len, NULL, NULL);
					result->vt = VT_DISPATCH;
					result->pdispVal = (std::make_unique<Buffer>(true, len, fData.release())).release();
					SysFreeString(data);
				}
			}
			else if (numArgs == 3) //length, fill, and encoding no impl
			{
				return E_NOTIMPL;
			}
			break;
		}
		case allocUnsafe:
		{
			auto numArgs = params->cArgs;
			if (!numArgs)
			{
				result->vt = VT_DISPATCH;
				result->pdispVal = (std::make_unique<Buffer>()).release();
			}
			else if (numArgs == 1) //length
			{
				auto len = params->rgvarg[0].intVal;
				result->vt = VT_DISPATCH;
				result->pdispVal = (std::make_unique<Buffer>(false, len)).release();
			}
			else if (numArgs == 2) //length and fill
			{
				auto len = params->rgvarg[0].intVal;
				auto data = params->rgvarg[1].bstrVal;
				int bstrLen = WideCharToMultiByte(CP_UTF8, 0, data, -1, NULL, 0, NULL, NULL);
				if (bstrLen)
				{
					auto fData = std::make_unique<byte[]>(bstrLen);
					WideCharToMultiByte(CP_UTF8, 0, data, -1, (LPSTR)fData.get(), len, NULL, NULL);
					result->vt = VT_DISPATCH;
					result->pdispVal = (std::make_unique<Buffer>(true, len, fData.release())).release();
					SysFreeString(data);
				}
			}
			break;
		}
		case allocUnsafeSlow: //our impl will run like the regular allocation atm. We may pool once the application is running, but all instances are separated atm.
		{
			auto numArgs = params->cArgs;
			if (!numArgs)
			{
				result->vt = VT_DISPATCH;
				result->pdispVal = (std::make_unique<Buffer>()).release();
			}
			else if (numArgs == 1) //length
			{
				auto len = params->rgvarg[0].intVal;
				result->vt = VT_DISPATCH;
				result->pdispVal = (std::make_unique<Buffer>(false, len)).release();
			}
			else if (numArgs == 2) //length and fill
			{
				auto len = params->rgvarg[0].intVal;
				auto data = params->rgvarg[1].bstrVal;
				int bstrLen = WideCharToMultiByte(CP_UTF8, 0, data, -1, NULL, 0, NULL, NULL);
				if (bstrLen)
				{
					auto fData = std::make_unique<byte[]>(bstrLen);
					WideCharToMultiByte(CP_UTF8, 0, data, -1, (LPSTR)fData.get(), len, NULL, NULL);
					result->vt = VT_DISPATCH;
					result->pdispVal = (std::make_unique<Buffer>(true, len, fData.release())).release();
					SysFreeString(data);
				}
			}
			break;
		}
		case byteLength: //right now only support the string type. Will support others as it comes in time
		{
			auto numArgs = params->cArgs;
			if (numArgs == 1)
			{
				auto data = params->rgvarg[0].bstrVal;
				int len = WideCharToMultiByte(CP_UTF8, 0, data, -1, NULL, 0, NULL, NULL);
				result->vt = VT_INT;
				result->intVal = len;
				SysFreeString(data);
			}
			else
			{
				return E_FAIL;
			}
			break;
		}
		case compare:
		{
			auto numArgs = params->cArgs;
			if (numArgs == 2) //again, assuming all values are correct atm
			{
				auto b1 = (Buffer)params->rgvarg[0].pdispVal;
				auto b2 = (Buffer)params->rgvarg[1].pdispVal;
				int ret = memcmp(b1.internArr->getBase(), b2.internArr->getBase(), sizeof(b1.internArr->getBase()));
				if (ret < 0)
				{
					result->vt = VT_INT;
					result->intVal = -1;
				}
				else if (ret > 0)
				{
					result->vt = VT_INT;
					result->intVal = 1;
				}
				else
				{
					result->vt = VT_INT;
					result->intVal = 0;
				}
			}
			else 
			{
				return E_FAIL;
			}
			break;
		}
		case concat:
		{
			auto numArgs = params->cArgs;
			if (numArgs == 1)
			{
				auto args = params->rgvarg[0].parray;
				Buffer* buffers;
				SafeArrayAccessData(args, (void**)&buffers);
				long lb, ub;
				SafeArrayGetLBound(args, 1, &lb);
				SafeArrayGetUBound(args, 1, &ub);
				long numElements = ub - lb + 1;
				long byteCount = 0;
				for (int i = 0; i < numElements; i++) {
					byteCount += sizeof(buffers[i].internArr->getBase());
				}
				byte* nBuf = new byte[byteCount];
				byteCount = 0;
				for (int i = 0; i < numElements; i++) {
					memcpy(nBuf, buffers[i].internArr->getBase(), sizeof(buffers[i].internArr->getBase()));
					nBuf += sizeof(buffers[i].internArr->getBase());
				}
				std::unique_ptr<Buffer> buf = std::make_unique<Buffer>(nBuf);
				result->vt = VT_DISPATCH;
				result->pdispVal = buf.release();
				SafeArrayUnaccessData(args);
			}
			else if (numArgs == 2)
			{
				auto args = params->rgvarg[0].parray;
				Buffer* buffers;
				SafeArrayAccessData(args, (void**)&buffers);
				long lb, ub;
				SafeArrayGetLBound(args, 1, &lb);
				SafeArrayGetUBound(args, 1, &ub);
				long numElements = ub - lb + 1;
				long byteCount = params->rgvarg[1].intVal;
				byte* nBuf = new byte[byteCount];
				for (int i = 0; i < numElements; i++) {
					memcpy(nBuf, buffers[i].internArr->getBase(), sizeof(buffers[i].internArr->getBase()));
					nBuf += sizeof(buffers[i].internArr->getBase());
				}
				std::unique_ptr<Buffer> buf = std::make_unique<Buffer>(nBuf);
				result->vt = VT_DISPATCH;
				result->pdispVal = buf.release();
				SafeArrayUnaccessData(args);
			}
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
			auto numArgs = params->cArgs;
			if (numArgs == 1) 
			{
				auto b = (Buffer)params->rgvarg[0].pdispVal;
				int ret = memcmp(b.internArr->getBase(), b.internArr->getBase(), sizeof(b.internArr->getBase()));
				if (ret == 0)
				{
					result->vt = VT_BOOL;
					result->boolVal = true;
				}
				else
				{
					result->vt = VT_BOOL;
					result->boolVal = false;
				}
			}
			else
			{
				return E_FAIL;
			}
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