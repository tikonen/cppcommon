#pragma once

#include <vector>
#include <intrin.h>
#include <stdexcept>

class BinaryPacker {
public:
	enum Endianess {
		LITTLE_ENDIAN,
		BIG_ENDIAN
	};

	BinaryPacker(DWORD size = 128)
	{
		mData.reserve(size);
	}

	template<typename T>
	BinaryPacker& add16bWord(T b, Endianess end)
	{
		return add((WORD)b, end);
	}

	template<typename T>
	BinaryPacker& add32bWord(T b, Endianess end)
	{
		return add((DWORD)b, end);
	}

	template<typename T>
	BinaryPacker& addByte(T b)
	{
		return add((BYTE)b, Endianess::LITTLE_ENDIAN);
	}

	BinaryPacker& add(const BYTE * pData, DWORD dwLen)
	{
		mData.insert(mData.end(), pData, pData + dwLen);
		return *this;
	}

	BinaryPacker& skip(DWORD dwLen)
	{
		return set(dwLen, 0x00);
	}

	BinaryPacker& set(DWORD dwLen, BYTE b)
	{
		mData.resize(mData.size() + dwLen, b);
		return *this;
	}

	void CopyTo(BYTE *pDst, DWORD &dwLen)
	{
		dwLen = min(dwLen, mData.size());
		memcpy(pDst, mData.data(), dwLen);
	}

	DWORD GetLength()
	{
		return mData.size();
	}
protected:
	union Data {
		BYTE bData;
		DWORD dwData;
		WORD wData;
		LONG64 qwData;
	};

	template <int>
	struct int_functor;

	template <>
	struct int_functor<1>
	{
		void operator()(BYTE b, Data *d, Endianess) const
		{
			d->bData = b;
		}
	};

	template <>
	struct int_functor<2>
	{
		void operator()(WORD w, Data *d, Endianess end) const
		{
			d->wData = end == LITTLE_ENDIAN ? w : _byteswap_ushort(w);
		}
	};

	template <>
	struct int_functor<4>
	{
		void operator()(DWORD dw, Data *d, Endianess end) const
		{
			d->dwData = end == LITTLE_ENDIAN ? dw : _byteswap_ulong(dw);
		}
	};

	template <>
	struct int_functor<8>
	{
		void operator()(LONG64 qw, Data *d, Endianess end) const
		{
			d->qwData = end == LITTLE_ENDIAN ? qw : _byteswap_uint64(qw);
		}
	};

	template <class T>
	BinaryPacker &add(T w, Endianess end)
	{
		Data d{};
		// pick correct conversion based on the size of T
		int_functor<sizeof(T)>()(w, &d, end);
		mData.insert(mData.end(), (BYTE*)&d, (BYTE*)&d + sizeof(T));
		return *this;
	}

	
private:
	std::vector<BYTE> mData;
};
