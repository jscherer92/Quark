#include "BaseObject.h"

class TypedArray : public BaseObject {
private:
	std::unique_ptr<byte[]> internalBuffer;
	HRESULT getMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) { return E_NOTIMPL; };
	HRESULT setMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) { return E_NOTIMPL; };
	//virtual HRESULT setReferenceMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) = 0;
	HRESULT createObject(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) { return E_NOTIMPL; };
	HRESULT callFunction(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) { return E_NOTIMPL; };
	std::map<memberType, std::vector<std::wstring>> memberMap = {
		{ GET,{} },
		{ SET,{} },
		{ CREATE,{} },
		{ CALL,{} }
	};
public:
	TypedArray(bool safe = true, long numBytes = 0, byte fill[] = { 0x00 }) : BaseObject(memberMap) {
		internalBuffer = std::make_unique<byte[]>(numBytes);
		if (safe)
		{
			//just going to fill with 0. will use fill buffer later
			std::fill_n(internalBuffer.get(), numBytes, 0x00);
		}
	};
	TypedArray(byte buffer[]) : BaseObject(memberMap) {
		internalBuffer = std::make_unique<byte[]>(sizeof(buffer));
		memcpy(internalBuffer.get(), buffer, sizeof(buffer));
	}

	byte* getBase() {
		return internalBuffer.get();
	}


};

class Buffer : public BaseObject {
private:
	virtual HRESULT getMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;
	virtual HRESULT setMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;
	//virtual HRESULT setReferenceMember(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) = 0;
	virtual HRESULT createObject(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;
	virtual HRESULT callFunction(DISPID id, std::unique_ptr<DISPPARAMS> params, std::unique_ptr<VARIANT> result, std::unique_ptr<EXCEPINFO> exception, std::unique_ptr<UINT> errArg) override;

	//internal representation of the buffer
	std::unique_ptr<TypedArray> internArr;
	enum getMethods { poolSize = 1, buffer = 2, length = 3, INSPECT_MAX_BYTES = 4, kMaxLength = 5 };
	enum type {
		DOUBLE = 24,
		FLOAT = 20,
		INT8 = 1,
		INT16 = 2,
		INT32 = 4,
		CUSTOM = 0
	};
	enum callMethods {
		alloc = 1,
		allocUnsafe = 2,
		allocUnsafeSlow = 3,
		byteLength = 4,
		compare = 5,
		concat = 6,
		from = 7,
		isBuffer = 8,
		isEncoding = 9,
		copy = 10,
		entries = 11,
		equals = 12,
		fill = 13,
		includes = 14,
		indexOf = 15,
		keys = 16,
		lastIndexOf = 17,
		readDoubleBE = 18,
		readDoubleLE = 19,
		readFloatBE = 20,
		readFloatLE = 21,
		readInt8 = 22,
		readInt16BE = 23,
		readInt16LE = 24,
		readInt32BE = 25,
		readInt32LE = 26,
		readIntBE = 27,
		readIntLE = 28,
		readUInt8 = 29,
		readUInt16BE = 30,
		readUInt16LE = 31,
		readUInt32BE = 32,
		readUInt32LE = 33,
		readUIntBE = 34,
		readUIntLE = 35,
		slice = 36,
		swap16 = 37,
		swap32 = 38,
		swap64 = 39,
		toJSON = 40,
		toString = 41,
		values = 42,
		write = 43,
		writeDoubleBE = 44,
		writeDoubleLE = 45,
		writeFloatBE = 46,
		writeFloatLE = 47,
		writeInt8 = 48,
		writeInt16BE = 49,
		writeInt16LE = 50,
		writeInt32BE = 51,
		writeInt32LE = 52,
		writeIntBE = 53,
		writeIntLE = 54,
		writeUInt8 = 55,
		writeUInt16BE = 56,
		writeUInt16LE = 57,
		writeUInt32BE = 58,
		writeUInt32LE = 59,
		writeUIntBE = 60,
		writeUIntLE = 61,
		transcode = 62
	};
	std::map<memberType, std::vector<std::wstring>> methodMap = {
		{ SET,{} },
		{ GET,{ L"poolSize", L"buffer", L"length", L"INSPECT_MAX_BYTES", L"kMaxLength" } },
		{ CREATE,{} },
		{ CALL,{
			L"alloc",
			L"allocUnsafe",
			L"allocUnsafeSlow",
			L"byteLength",
			L"compare",
			L"concat",
			L"from",
			L"isBuffer",
			L"isEncoding",
			L"copy",
			L"entries",
			L"equals",
			L"fill",
			L"includes",
			L"indexOf",
			L"keys",
			L"lastIndexOf",
			L"readDoubleBE",
			L"readDoubleLE",
			L"readFloatBE",
			L"readFloatLE",
			L"readInt8",
			L"readInt16BE",
			L"readInt16LE",
			L"readInt32BE",
			L"readInt32LE",
			L"readIntBE",
			L"readIntLE",
			L"readUInt8",
			L"readUInt16BE",
			L"readUInt16LE",
			L"readUInt32BE",
			L"readUInt32LE",
			L"readUIntBE",
			L"readUIntLE",
			L"slice",
			L"swap16",
			L"swap32",
			L"swap64",
			L"toJSON",
			L"toString",
			L"values",
			L"write",
			L"writeDoubleBE",
			L"writeDoubleLE",
			L"writeFloatBE",
			L"writeFloatLE",
			L"writeInt8",
			L"writeInt16BE",
			L"writeInt16LE",
			L"writeInt32BE",
			L"writeInt32LE",
			L"writeIntBE",
			L"writeIntLE",
			L"writeUint8",
			L"writeUInt16BE",
			L"writeUInt16LE",
			L"writeUInt32BE",
			L"writeUInt32LE",
			L"writeUIntBE",
			L"writeUIntLE",
			L"transcode"
		} }
	};
protected:
	bool safeAccess(int offset, type type = INT32, int num = -1);
public:
	Buffer(bool safe = true, long numBytes = 0, byte fill[] = { 0x00 }) : BaseObject(methodMap) {
		internArr = std::make_unique<TypedArray>(safe, numBytes, fill);
	}
	Buffer(byte buffer[]) : BaseObject(methodMap) {
		internArr = std::make_unique<TypedArray>(buffer);
	}
};