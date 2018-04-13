#include "Buffer.h";

bool Buffer::safeAccess(int offset, type type = INT32, int num = -1) {
	int len = 0;
	if (type == CUSTOM)
	{
		len = num;
	}
	else if (type & 16 == 1)
	{
		len = type % 16;
	}
	else
	{
		len = type;
	}

	return (((offset + 1) * len) > sizeof(this->internArr->getBase()));
}

void Buffer::_copy(std::unique_ptr<Buffer> target, long targetStart, long sourceStart, long sourceEnd)
{
	std::memcpy(target.get()->internArr->getBase() + targetStart, this->internArr->getBase() + sourceStart, sourceEnd - sourceEnd);
}

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
			auto numArgs = params->cArgs;
			if (numArgs == 1) //only have target
				this->_copy(std::make_unique<Buffer>(params->rgvarg[0].pdispVal));
			else if (numArgs == 2)
				this->_copy(std::make_unique<Buffer>(params->rgvarg[0].pdispVal), params->rgvarg[1].lVal);
			else if (numArgs == 3)
				this->_copy(std::make_unique<Buffer>(params->rgvarg[0].pdispVal), params->rgvarg[1].lVal, params->rgvarg[2].lVal);
			else
				this->_copy(std::make_unique<Buffer>(params->rgvarg[0].pdispVal), params->rgvarg[1].lVal, params->rgvarg[2].lVal, params->rgvarg[3].lVal);
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
			//do check to see if we are outside of our location
			if (params->cArgs == 1)
			{
				int offset = params->rgvarg[0].intVal;
				if (this->safeAccess(offset, DOUBLE))
				{
					double val = this->internArr->getBase()[offset];
					result->vt = VT_R8;
					result->dblVal = val;
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				int offset = params->rgvarg[0].intVal;
				bool check = params->rgvarg[1].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, DOUBLE))
					{
						double val = this->internArr->getBase()[offset];
						result->vt = VT_R8;
						result->dblVal = val;
					}
					else
					{
						return E_FAIL;
					}
				} 
				else
				{
					double val = this->internArr->getBase()[offset];
					result->vt = VT_R8;
					result->dblVal = val;
				}
			}
			break;
		}
		case readDoubleLE:
		{
			union { byte b[8]; double d; }; //don't know if this is the best use but going to do it anyway! :)
			//do check to see if we are outside of our location
			if (params->cArgs == 1)
			{
				int offset = params->rgvarg[0].intVal;
				if (this->safeAccess(offset, DOUBLE))
				{
					//genericize this!!!
					byte* loc = this->internArr->getBase();
					for (auto i = offset + 7, j = 0; i >= offset; i--, j++)
					{
						b[j] = loc[i];
					}
					result->vt = VT_R8;
					result->dblVal = d;
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				int offset = params->rgvarg[0].intVal;
				bool check = params->rgvarg[1].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, DOUBLE))
					{
						// genericize this!!!
						byte* loc = this->internArr->getBase();
						for (auto i = offset + 7, j = 0; i >= offset; i--, j++)
						{
							b[j] = loc[i];
						}
						result->vt = VT_R8;
						result->dblVal = d;
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					// genericize this!!!
					byte* loc = this->internArr->getBase();
					for (auto i = offset + 7, j = 0; i >= offset; i--, j++)
					{
						b[j] = loc[i];
					}
					result->vt = VT_R8;
					result->dblVal = d;
				}
			}
			break;
		}
		case readFloatBE:
		{
			//do check to see if we are outside of our location
			if (params->cArgs == 1)
			{
				int offset = params->rgvarg[0].intVal;
				if (this->safeAccess(offset, FLOAT))
				{
					float val = this->internArr->getBase()[offset];
					result->vt = VT_R4;
					result->fltVal = val;
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				int offset = params->rgvarg[0].intVal;
				bool check = params->rgvarg[1].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, FLOAT))
					{
						float val = this->internArr->getBase()[offset];
						result->vt = VT_R4;
						result->fltVal = val;
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					float val = this->internArr->getBase()[offset];
					result->vt = VT_R4;
					result->fltVal = val;
				}
			}
			break;
		}
		case readFloatLE:
		{
			union { byte b[4]; float f; };
			if (params->cArgs == 1)
			{
				int offset = params->rgvarg[0].intVal;
				if (this->safeAccess(offset, FLOAT))
				{
					//genericize this!!!
					byte* loc = this->internArr->getBase();
					for (auto i = offset + 3, j = 0; i >= offset; i--, j++)
					{
						b[j] = loc[i];
					}
					result->vt = VT_R4;
					result->fltVal = f;
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				int offset = params->rgvarg[0].intVal;
				bool check = params->rgvarg[1].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, DOUBLE))
					{
						// genericize this!!!
						byte* loc = this->internArr->getBase();
						for (auto i = offset + 3, j = 0; i >= offset; i--, j++)
						{
							b[j] = loc[i];
						}
						result->vt = VT_R4;
						result->fltVal = f;
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					// genericize this!!!
					byte* loc = this->internArr->getBase();
					for (auto i = offset + 3, j = 0; i >= offset; i--, j++)
					{
						b[j] = loc[i];
					}
					result->vt = VT_R4;
					result->fltVal = f;
				}
			}
			break;
		}
		case readInt8:
		{
			if (params->cArgs == 1)
			{
				int offset = params->rgvarg[0].intVal;
				if (this->safeAccess(offset, INT8))
				{
					byte val = this->internArr->getBase()[offset];
					result->vt = VT_I1;
					result->bVal = val;
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				int offset = params->rgvarg[0].intVal;
				bool check = params->rgvarg[1].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, INT8))
					{
						byte val = this->internArr->getBase()[offset];
						result->vt = VT_I1;
						result->bVal = val;
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					byte val = this->internArr->getBase()[offset];
					result->vt = VT_I1;
					result->bVal = val;
				}
			}
			break;
		}
		case readInt16BE:
		{
			if (params->cArgs == 1)
			{
				int offset = params->rgvarg[0].intVal;
				if (this->safeAccess(offset, INT16))
				{
					short val = this->internArr->getBase()[offset];
					result->vt = VT_I2;
					result->iVal = val;
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				int offset = params->rgvarg[0].intVal;
				bool check = params->rgvarg[1].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, INT16))
					{
						short val = this->internArr->getBase()[offset];
						result->vt = VT_I2;
						result->iVal = val;
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					short val = this->internArr->getBase()[offset];
					result->vt = VT_I2;
					result->iVal = val;
				}
			}
			break;
		}
		case readInt16LE:
		{
			union { byte b[2]; short s; };
			if (params->cArgs == 1)
			{
				int offset = params->rgvarg[0].intVal;
				if (this->safeAccess(offset, INT16))
				{
					//genericize this!!!
					byte* loc = this->internArr->getBase();
					for (auto i = offset + 1, j = 0; i >= offset; i--, j++)
					{
						b[j] = loc[i];
					}
					result->vt = VT_I2;
					result->iVal = s;
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				int offset = params->rgvarg[0].intVal;
				bool check = params->rgvarg[1].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, INT16))
					{
						// genericize this!!!
						byte* loc = this->internArr->getBase();
						for (auto i = offset + 1, j = 0; i >= offset; i--, j++)
						{
							b[j] = loc[i];
						}
						result->vt = VT_I4;
						result->iVal = s;
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					// genericize this!!!
					byte* loc = this->internArr->getBase();
					for (auto i = offset + 1, j = 0; i >= offset; i--, j++)
					{
						b[j] = loc[i];
					}
					result->vt = VT_I4;
					result->iVal = s;
				}
			}
			break;
		}
		case readInt32BE:
		{
			if (params->cArgs == 1)
			{
				int offset = params->rgvarg[0].intVal;
				if (this->safeAccess(offset, INT32))
				{
					long val = this->internArr->getBase()[offset];
					result->vt = VT_I4;
					result->lVal = val;
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				int offset = params->rgvarg[0].intVal;
				bool check = params->rgvarg[1].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, INT32))
					{
						long val = this->internArr->getBase()[offset];
						result->vt = VT_I4;
						result->lVal = val;
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					long val = this->internArr->getBase()[offset];
					result->vt = VT_I4;
					result->lVal = val;
				}
			}
			break;
		}
		case readInt32LE:
		{
			union { byte b[4]; int s; };
			if (params->cArgs == 1)
			{
				int offset = params->rgvarg[0].intVal;
				if (this->safeAccess(offset, INT32))
				{
					//genericize this!!!
					byte* loc = this->internArr->getBase();
					for (auto i = offset + 3, j = 0; i >= offset; i--, j++)
					{
						b[j] = loc[i];
					}
					result->vt = VT_I4;
					result->iVal = s;
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				int offset = params->rgvarg[0].intVal;
				bool check = params->rgvarg[1].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, INT32))
					{
						// genericize this!!!
						byte* loc = this->internArr->getBase();
						for (auto i = offset + 3, j = 0; i >= offset; i--, j++)
						{
							b[j] = loc[i];
						}
						result->vt = VT_I4;
						result->iVal = s;
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					// genericize this!!!
					byte* loc = this->internArr->getBase();
					for (auto i = offset + 3, j = 0; i >= offset; i--, j++)
					{
						b[j] = loc[i];
					}
					result->vt = VT_I4;
					result->iVal = s;
				}
			}
			break;
		}
		case readIntBE:
		{
			if (params->cArgs == 2)
			{
				int offset = params->rgvarg[0].intVal;
				int bl = params->rgvarg[1].intVal;
				if (this->safeAccess(offset, CUSTOM, bl))
				{
					long val = this->internArr->getBase()[offset];
					result->vt = VT_I4;
					result->lVal = val;
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				int offset = params->rgvarg[0].intVal;
				int bl = params->rgvarg[1].intVal;
				bool check = params->rgvarg[2].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, CUSTOM, bl))
					{
						long val = this->internArr->getBase()[offset];
						result->vt = VT_I4;
						result->lVal = val;
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					long val = this->internArr->getBase()[offset];
					result->vt = VT_I4;
					result->lVal = val;
				}
			}
			break;
		}
		case readIntLE:
		{
			
		}
		case slice:
		{
			break;
		}
		case swap16:
		{
			if (sizeof(this->internArr->getBase()) % 2 != 0)
			{
				return E_FAIL;
			} 
			else
			{
				for (auto i = 0; i < sizeof(this->internArr->getBase()); i += 2) {
					byte t = this->internArr->getBase()[i];
					this->internArr->getBase()[i] = this->internArr->getBase()[i + 1];
					this->internArr->getBase()[i + 1] = t;
				}
			}
			break;
		}
		case swap32:
		{
			if (sizeof(this->internArr->getBase()) % 4 != 0)
			{
				return E_FAIL;
			}
			else
			{
				byte* temp = this->internArr->getBase();
				for (auto i = 0; i < sizeof(temp); i += 4)
				{
					byte t1 = temp[i];
					byte t2 = temp[i + 1];
					temp[i] = temp[i + 3];
					temp[i + 1] = temp[i + 2];
					temp[i + 2] = t2;
					temp[i + 3] = t1;
				}
			}
			break;
		}
		case swap64:
		{
			byte* temp = this->internArr->getBase();
			for (auto i = 0; i < sizeof(temp); i += 6)
			{
				byte t1 = temp[i];
				byte t2 = temp[i + 1];
				byte t3 = temp[i + 2];
				temp[i] = temp[i + 5];
				temp[i + 1] = temp[i + 4];
				temp[i + 2] = temp[i + 3];
				temp[i + 3] = t3;
				temp[i + 4] = t2;
				temp[i + 5] = t1;
			}
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
		case write: //currently only support utf8 atm
		{
			auto numArgs = params->cArgs;
			if (numArgs == 1)
			{
				BSTR data = params->rgvarg[0].bstrVal;
				int bstrLen = WideCharToMultiByte(CP_UTF8, 0, data, -1, NULL, 0, NULL, NULL);
				if (bstrLen)
				{
					auto fData = std::make_unique<byte[]>(bstrLen);
					WideCharToMultiByte(CP_UTF8, 0, data, -1, (LPSTR)fData.get(), bstrLen, NULL, NULL);
					byte* b = this->internArr->getBase();
					long count = 0;
					for (auto i = 0; i < bstrLen; i++)
					{
						if (i != sizeof(fData.get())) {
							b[i] = fData.get()[i];
						}
						else {
							break;
						}
						count++;
					}
					result->vt = VT_I4;
					result->lVal = count;
					SysFreeString(data);
				}
			}
			else if (numArgs == 2)
			{
				BSTR data = params->rgvarg[0].bstrVal;
				long loc = params->rgvarg[1].lVal;
				int bstrLen = WideCharToMultiByte(CP_UTF8, 0, data, -1, NULL, 0, NULL, NULL);
				if (bstrLen)
				{
					auto fData = std::make_unique<byte[]>(bstrLen);
					WideCharToMultiByte(CP_UTF8, 0, data, -1, (LPSTR)fData.get(), bstrLen, NULL, NULL);
					byte* b = this->internArr->getBase();
					long count = 0;
					for (auto i = 0; i < bstrLen; i++)
					{
						if (i + loc != sizeof(fData.get())) {
							b[i + loc] = fData.get()[i];
						}
						else {
							break;
						}
						count++;
					}
					result->vt = VT_I4;
					result->lVal = count;
					SysFreeString(data);
				}
			}
			else if (numArgs == 3)
			{
				BSTR data = params->rgvarg[0].bstrVal;
				long loc = params->rgvarg[1].lVal;
				long length = params->rgvarg[2].lVal;
				int bstrLen = WideCharToMultiByte(CP_UTF8, 0, data, -1, NULL, 0, NULL, NULL);
				if (bstrLen)
				{
					auto fData = std::make_unique<byte[]>(bstrLen);
					WideCharToMultiByte(CP_UTF8, 0, data, -1, (LPSTR)fData.get(), bstrLen, NULL, NULL);
					byte* b = this->internArr->getBase();
					long count = 0;
					for (auto i = 0; i < length; i++)
					{
						if (i + loc != sizeof(fData.get())) {
							b[i + loc] = fData.get()[i];
						}
						else {
							break;
						}
						count++;
					}
					result->vt = VT_I4;
					result->lVal = count;
					SysFreeString(data);
				}
			}
			break;
		}
		case writeDoubleBE:
		{
			if (params->cArgs == 2)
			{
				double val = params->rgvarg[0].dblVal;
				int offset = params->rgvarg[1].intVal;
				if (this->safeAccess(offset, DOUBLE))
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				double val = params->rgvarg[0].dblVal;
				int offset = params->rgvarg[1].intVal;
				bool check = params->rgvarg[2].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, DOUBLE))
					{
						this->internArr->getBase()[offset] = val;
						result->vt = VT_I4;
						result->lVal = sizeof(val);
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
			}
			break;
		}
		case writeDoubleLE:
		{
			union { byte b[8]; double d; };
			if (params->cArgs == 2)
			{
				double val = params->rgvarg[0].dblVal;
				int offset = params->rgvarg[1].intVal;
				if (this->safeAccess(offset, DOUBLE))
				{
					d = val;
					for (auto i = 7, j = 0; i <= 0; i++, j++)
					{
						this->internArr->getBase()[offset + j] = b[i];
					}
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				double val = params->rgvarg[0].dblVal;
				int offset = params->rgvarg[1].intVal;
				bool check = params->rgvarg[2].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, DOUBLE))
					{
						d = val;
						for (auto i = 7, j = 0; i <= 0; i++, j++)
						{
							this->internArr->getBase()[offset + j] = b[i];
						}
						result->vt = VT_I4;
						result->lVal = sizeof(val);
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					d = val;
					for (auto i = 7, j = 0; i <= 0; i++, j++)
					{
						this->internArr->getBase()[offset + j] = b[i];
					}
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
			}
			break;
		}
		case writeFloatBE:
		{
			if (params->cArgs == 2)
			{
				float val = params->rgvarg[0].fltVal;
				int offset = params->rgvarg[1].intVal;
				if (this->safeAccess(offset, FLOAT))
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				float val = params->rgvarg[0].fltVal;
				int offset = params->rgvarg[1].intVal;
				bool check = params->rgvarg[2].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, FLOAT))
					{
						this->internArr->getBase()[offset] = val;
						result->vt = VT_I4;
						result->lVal = sizeof(val);
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
			}
			break;
		}
		case writeFloatLE:
		{
			union { byte b[4]; float f; };
			if (params->cArgs == 2)
			{
				float val = params->rgvarg[0].fltVal;
				int offset = params->rgvarg[1].intVal;
				if (this->safeAccess(offset, FLOAT))
				{
					f = val;
					for (auto i = 3, j = 0; i >= 0; i--, j++)
					{
						this->internArr->getBase()[offset + j] = b[i];
					}
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				float val = params->rgvarg[0].fltVal;
				int offset = params->rgvarg[1].intVal;
				bool check = params->rgvarg[2].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, FLOAT))
					{
						f = val;
						for (auto i = 3, j = 0; i >= 0; i--, j++)
						{
							this->internArr->getBase()[offset + j] = b[i];
						}
						result->vt = VT_I4;
						result->lVal = sizeof(val);
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					f = val;
					for (auto i = 3, j = 0; i >= 0; i--, j++)
					{
						this->internArr->getBase()[offset + j] = b[i];
					}
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
			}
			break;
		}
		case writeInt8:
		{
			if (params->cArgs == 2)
			{
				byte val = params->rgvarg[0].cVal;
				int offset = params->rgvarg[1].intVal;
				if (this->safeAccess(offset, INT8))
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				byte val = params->rgvarg[0].cVal;
				int offset = params->rgvarg[1].intVal;
				bool check = params->rgvarg[2].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, INT8))
					{
						this->internArr->getBase()[offset] = val;
						result->vt = VT_I4;
						result->lVal = sizeof(val);
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
			}
			break;
		}
		case writeInt16BE:
		{
			if (params->cArgs == 2)
			{
				short val = params->rgvarg[0].iVal;
				int offset = params->rgvarg[1].intVal;
				if (this->safeAccess(offset, INT16))
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				short val = params->rgvarg[0].iVal;
				int offset = params->rgvarg[1].intVal;
				bool check = params->rgvarg[2].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, INT16))
					{
						this->internArr->getBase()[offset] = val;
						result->vt = VT_I4;
						result->lVal = sizeof(val);
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
			}
			break;
		}
		case writeInt16LE:
		{
			union { byte b[2]; short s; };
			if (params->cArgs == 2)
			{
				short val = params->rgvarg[0].iVal;
				int offset = params->rgvarg[1].intVal;
				if (this->safeAccess(offset, INT16))
				{
					s = val;
					for (auto i = 1, j = 0; i >= 0; i--, j++)
					{
						this->internArr->getBase()[offset + j] = s[i];
					}
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				short val = params->rgvarg[0].iVal;
				int offset = params->rgvarg[1].intVal;
				bool check = params->rgvarg[2].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, INT16))
					{
						s = val;
						for (auto i = 1, j = 0; i >= 0; i--, j++)
						{
							this->internArr->getBase()[offset + j] = s[i];
						}
						result->vt = VT_I4;
						result->lVal = sizeof(val);
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					s = val;
					for (auto i = 1, j = 0; i >= 0; i--, j++)
					{
						this->internArr->getBase()[offset + j] = s[i];
					}
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
			}
			break;
		}
		case writeInt32BE:
		{
			if (params->cArgs == 2)
			{
				long val = params->rgvarg[0].lVal;
				int offset = params->rgvarg[1].intVal;
				if (this->safeAccess(offset, INT32))
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				long val = params->rgvarg[0].lVal;
				int offset = params->rgvarg[1].intVal;
				bool check = params->rgvarg[2].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, INT32))
					{
						this->internArr->getBase()[offset] = val;
						result->vt = VT_I4;
						result->lVal = sizeof(val);
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
			}
			break;
		}
		case writeInt32LE:
		{
			union { byte b[4]; int s; };
			if (params->cArgs == 2)
			{
				long val = params->rgvarg[0].lVal;
				int offset = params->rgvarg[1].intVal;
				if (this->safeAccess(offset, INT32))
				{
					s = val;
					for (auto i = 3, j = 0; i >= 0; i--, j++)
					{
						this->internArr->getBase()[offset + j] = b[i];
					}
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				long val = params->rgvarg[0].lVal;
				int offset = params->rgvarg[1].intVal;
				bool check = params->rgvarg[2].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, INT32))
					{
						s = val;
						for (auto i = 3, j = 0; i >= 0; i--, j++)
						{
							this->internArr->getBase()[offset + j] = b[i];
						}
						result->vt = VT_I4;
						result->lVal = sizeof(val);
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					s = val;
					for (auto i = 3, j = 0; i >= 0; i--, j++)
					{
						this->internArr->getBase()[offset + j] = b[i];
					}
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
			}
			break;
		}
		case writeIntBE:
		{
			if (params->cArgs == 3)
			{
				long val = params->rgvarg[0].lVal;
				int offset = params->rgvarg[1].intVal;
				int bl = params->rgvarg[2].intVal;
				if (this->safeAccess(offset, CUSTOM, bl))
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = val;
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				long val = params->rgvarg[0].lVal;
				int offset = params->rgvarg[1].intVal;
				int bl = params->rgvarg[2].intVal;
				bool check = params->rgvarg[3].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, CUSTOM, bl))
					{
						this->internArr->getBase()[offset] = val;
						result->vt = VT_I4;
						result->lVal = val;
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = val;
				}
			}
			break;
		}
		case writeIntLE:
		{
			break;
		}
		case writeUInt8:
		{
			if (params->cArgs == 2)
			{
				unsigned char val = params->rgvarg[0].bVal;
				int offset = params->rgvarg[1].intVal;
				if (this->safeAccess(offset, INT8))
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				unsigned char val = params->rgvarg[0].bVal;
				int offset = params->rgvarg[1].intVal;
				bool check = params->rgvarg[2].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, INT8))
					{
						this->internArr->getBase()[offset] = val;
						result->vt = VT_I4;
						result->lVal = sizeof(val);
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
			}
			break;
		}
		case writeUInt16BE:
		{
			if (params->cArgs == 2)
			{
				unsigned short val = params->rgvarg[0].uiVal;
				int offset = params->rgvarg[1].intVal;
				if (this->safeAccess(offset, INT16))
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				unsigned short val = params->rgvarg[0].uiVal;
				int offset = params->rgvarg[1].intVal;
				bool check = params->rgvarg[2].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, INT16))
					{
						this->internArr->getBase()[offset] = val;
						result->vt = VT_I4;
						result->lVal = sizeof(val);
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
			}
			break;
		}
		case writeUInt16LE:
		{
			break;
		}
		case writeUInt32BE:
		{
			if (params->cArgs == 2)
			{
				unsigned long val = params->rgvarg[0].ulVal;
				int offset = params->rgvarg[1].intVal;
				if (this->safeAccess(offset, INT32))
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				unsigned long val = params->rgvarg[0].uiVal;
				int offset = params->rgvarg[1].intVal;
				bool check = params->rgvarg[2].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, INT32))
					{
						this->internArr->getBase()[offset] = val;
						result->vt = VT_I4;
						result->lVal = sizeof(val);
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = sizeof(val);
				}
			}
			break;
		}
		case writeUInt32LE:
		{
			break;
		}
		case writeUIntBE:
		{
			if (params->cArgs == 3)
			{
				unsigned long val = params->rgvarg[0].ulVal;
				int offset = params->rgvarg[1].intVal;
				int bl = params->rgvarg[2].intVal;
				if (this->safeAccess(offset, CUSTOM, bl))
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = val;
				}
				else
				{
					return E_FAIL;
				}
			}
			else {
				unsigned long val = params->rgvarg[0].ulVal;
				int offset = params->rgvarg[1].intVal;
				int bl = params->rgvarg[2].intVal;
				bool check = params->rgvarg[3].boolVal;
				if (check)
				{
					if (this->safeAccess(offset, CUSTOM, bl))
					{
						this->internArr->getBase()[offset] = val;
						result->vt = VT_I4;
						result->lVal = val;
					}
					else
					{
						return E_FAIL;
					}
				}
				else
				{
					this->internArr->getBase()[offset] = val;
					result->vt = VT_I4;
					result->lVal = val;
				}
			}
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