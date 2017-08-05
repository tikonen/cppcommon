#pragma once

#pragma once

#include <Endianess.h>
#include <stdexcept>

#ifndef _MSC_VER
#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define LONG64 unsigned long long
#endif

namespace _BinaryUnPackerInternal
{
    union Data {
        BYTE bData;
        DWORD dwData;
        WORD wData;
        LONG64 qwData;
    };

    template <int> struct int_functor;

    template <> struct int_functor<1> {
        BYTE operator()(Data* d, bool) const { return d->bData; }
    };

    template <> struct int_functor<2> {
        WORD operator()(Data* d, bool le) const
        {
            return le ? le16toh(d->wData) : be16toh(d->wData);
        }
    };

    template <> struct int_functor<4> {
        DWORD operator()(Data* d, bool le) const
        {
            return le ? le32toh(d->dwData) : be32toh(d->dwData);
        }
    };

    template <> struct int_functor<8> {
        LONG64 operator()(Data* d, bool le) const
        {
            return le ? le64toh(d->qwData) : be64toh(d->qwData);
        }
    };
}

class BinaryUnPacker
{
public:
    BinaryUnPacker(BYTE* pData, DWORD dwLength)
    {
        mpData = pData;
        mLength = dwLength;
    }

    template <typename T> BinaryUnPacker& read16bWord(T* b, int end)
    {
        return readNumber((WORD*)b, end);
    }

    template <typename T> BinaryUnPacker& read32bWord(T* b, int end)
    {
        return readNumber((DWORD*)b, end);
    }

    template <typename T> BinaryUnPacker& readByte(T* b)
    {
        return readNumber((BYTE*)b, LITTLE_ENDIAN);
    }

    BinaryUnPacker& read(BYTE* pData, DWORD dwLen)
    {
        if (ensure(dwLen)) {
            memcpy(pData, mpData + mOffset, dwLen);
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

    DWORD GetLength() { return mLength; }

    DWORD GetOffset() { return mOffset; }

protected:
    bool ensure(DWORD len)
    {
        if (mOffset + len > mLength) {
            throw std::invalid_argument("not enough bytes");
        }
        return true;
    }

    template <class T>
    typename std::enable_if<std::is_integral<T>::value, BinaryUnPacker&>::type readNumber(T* w, int end)
    {
        if (ensure(sizeof(T))) {
            auto* d = (_BinaryUnPackerInternal::Data*)(mpData + mOffset);
            // pick correct conversion based on the size of T
            *w = _BinaryUnPackerInternal::int_functor<sizeof(T)>()(d, end == LITTLE_ENDIAN);
            mOffset += sizeof(T);
        }
        return *this;
    }

private:
    BYTE* mpData = nullptr;
    DWORD mLength = 0;
    DWORD mOffset = 0;
};
