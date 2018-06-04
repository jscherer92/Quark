/*
* ByteType Class gives a generic view of bytes to regular data types
* Especially when working with streams, these types will suffice to
* actually convert between them and by grabbing data from different parts of the stream/buffer
*/
class ByteType {
public:
	union DoubleByte {
		double d;
		unsigned char dByte[8];
	};
	union FloatByte {
		float l;
		unsigned char fByte[4];
	};
	union LongByte {
		long l;
		unsigned char lByte[8];
	};
	union UnsignedLongByte {
		unsigned long ul;
		unsigned char ulByte[8];
	};
	union IntByte {
		int i;
		unsigned char iByte[4];
	};
	union UnsignedIntByte {
		unsigned int ui;
		unsigned char uiByte[4];
	};
	union ShortByte {
		short s;
		unsigned char sByte[2];
	};
	union UnsignedShortByte {
		unsigned short us;
		unsigned char usByte[2];
	};
	union ByteByte{
		unsigned char b;
		unsigned char bByte[1];
	};
	/*
	* From here, making arrays of these types should be made. Should the use of the variant type in C++17 used or something else?
	* Need to think about that since this needs to be compiled across multiple systems. Should probably stick to 14 and below?
	*/
};