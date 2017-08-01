#pragma once

#pragma once

#include <intrin.h>
#include <stdexcept>

class BinaryUnPacker {
public:
	enum Endianess {
		LITTLE_ENDIAN,
		BIG_ENDIAN
	};

	BinaryUnPacker(BYTE *pData, DWORD dwLength)
	{
		mpData = pData;
		mLength = dwLength;
	}

	template<typename T>
	BinaryUnPacker& read16bWord(T *b, Endianess end)
	{
		return read((WORD*)b, end);
	}

	template<typename T>
	BinaryUnPacker& read32bWord(T *b, Endianess end)
	{
		return read((DWORD*)b, end);
	}

	template<typename T>
	BinaryUnPacker& readByte(T *b)
	{
		return read((BYTE*)b, Endianess::LITTLE_ENDIAN);
	}

	BinaryUnPacker& read(BYTE * pData, DWORD dwLen)
	{
		if (ensure(dwLen)) {
			memcpy(pData, pData + mOffset, dwLen);
			mOffset += dwLen;			
		}
		return *this;
	}

	BinaryUnPacker& skip(DWORD dwLen)
	{
		if (ensure(dwLen)) {
			mOffset += dwLen;
		}
		return *this;
	}

	DWORD GetLength()
	{
		return mLength;
	}

	DWORD GetOffset()
	{
		return mOffset;
	}

protected:
	union Data {
		BYTE bData;
		DWORD dwData;
		WORD wData;
		LONG64 qwData;
	};

	bool ensure(DWORD len)
	{
		if (mOffset + len > mLength) {
			throw std::invalid_argument("not enough bytes");
		}
		return true;		
	}

	template <int>
	struct int_functor;

	template <>
	struct int_functor<1>
	{
		BYTE operator()(Data *d, Endianess) const
		{
			return d->bData;
		}
	};

	template <>
	struct int_functor<2>
	{
		WORD operator()(Data *d, Endianess end) const
		{
			return end == LITTLE_ENDIAN ? d->wData : _byteswap_ushort(d->wData);
		}
	};

	template <>
	struct int_functor<4>
	{
		DWORD operator()(Data *d, Endianess end) const
		{
			return end == LITTLE_ENDIAN ? d->dwData : _byteswap_ulong(d->dwData);
		}
	};

	template <>
	struct int_functor<8>
	{
		LONG64 operator()(Data *d, Endianess end) const
		{
			return end == LITTLE_ENDIAN ? d->qwData : _byteswap_uint64(d->qwData);
		}
	};

	template <class T>
	BinaryUnPacker &read(T *w, Endianess end)
	{		
		if (ensure(sizeof(T))) {
			Data *d = (Data*)(mpData + mOffset);
			// pick correct conversion based on the size of T
			*w = int_functor<sizeof(T)>()(d, end);			
			mOffset += sizeof(T);
		}
		return *this;
	}

private:
	BYTE *mpData = nullptr;
	DWORD mLength = 0;
	DWORD mOffset = 0;
};
