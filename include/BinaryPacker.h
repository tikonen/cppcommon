#pragma once

#include <Endianess.h>
#include <vector>

#ifndef _MSC_VER
#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define LONG64 unsigned long long
#endif

namespace _BinaryPackerInternal
{
    union Data {
        BYTE bData;
        DWORD dwData;
        WORD wData;
        LONG64 qwData;
    };

    template <int> struct int_functor;

    template <> struct int_functor<1> {
        void operator()(BYTE b, Data* d, bool) const { d->bData = b; }
    };

    template <> struct int_functor<2> {
        void operator()(WORD w, Data* d, bool le) const { d->wData = le ? htole16(w) : htobe16(w); }
    };


    template <> struct int_functor<4> {
        void operator()(DWORD dw, Data* d, bool le) const
        {
            d->dwData = le ? htole32(dw) : htobe32(dw);
        }
    };

    template <> struct int_functor<8> {
        void operator()(LONG64 qw, Data* d, bool le) const
        {
            d->qwData = le ? htole64(qw) : htobe64(qw);
        }
    };
}

class BinaryPacker
{
public:
    BinaryPacker(int size = 128) { mData.reserve(size); }

    template <typename T> BinaryPacker& add16bWord(T b, int end) { return addNumber((WORD)b, end); }

    template <typename T> BinaryPacker& add32bWord(T b, int end)
    {
        return addNumber((DWORD)b, end);
    }

    template <typename T> BinaryPacker& addByte(T b) { return addNumber((BYTE)b, LITTLE_ENDIAN); }

    BinaryPacker& add(const BYTE* pData, int dwLen)
    {
        mData.insert(mData.end(), pData, pData + dwLen);
        return *this;
    }

    BinaryPacker& skip(int dwLen) { return set(dwLen, 0x00); }

    BinaryPacker& set(int dwLen, BYTE b)
    {
        mData.resize(mData.size() + dwLen, b);
        return *this;
    }

    void CopyTo(BYTE* pDst, int& dwLen)
    {
        dwLen = std::min(static_cast<size_t>(dwLen), mData.size());
        memcpy(pDst, mData.data(), dwLen);
    }

    int GetLength() { return mData.size(); }

protected:
    // make sure that this function is used only for integral types
    template <class T>
    typename std::enable_if<std::is_integral<T>::value, BinaryPacker&>::type addNumber(T w, int end)
    {
        _BinaryPackerInternal::Data d{};
        // pick correct conversion based on the size of T
        _BinaryPackerInternal::int_functor<sizeof(T)>()(w, &d, end == LITTLE_ENDIAN);
        mData.insert(mData.end(), (unsigned char*)&d, (unsigned char*)&d + sizeof(T));
        return *this;
    }

private:
    std::vector<BYTE> mData;
};
