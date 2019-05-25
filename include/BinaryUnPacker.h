#pragma once

#include "_BinaryPackerInternal.h"

#include <stdexcept>

class BinaryUnPacker
{
public:
    BinaryUnPacker(BYTE* pData, size_t dwLength)
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

    size_t GetLength() { return mLength; }

    size_t GetOffset() { return mOffset; }

protected:
    bool ensure(DWORD len)
    {
        if (mOffset + len > mLength) {
            throw std::out_of_range("not enough bytes");
        }
        return true;
    }

    template <class T>
    typename std::enable_if<std::is_integral<T>::value, BinaryUnPacker&>::type readNumber(T* w, int end)
    {
        if (ensure(sizeof(T))) {
            auto* d = (_BinaryPackerInternal::Data*)(mpData + mOffset);
            // pick correct conversion based on the size of T
            *w = _BinaryPackerInternal::int_functor<sizeof(T)>()(d, end == LITTLE_ENDIAN);
            mOffset += sizeof(T);
        }
        return *this;
    }

private:
    BYTE* mpData = nullptr;
    size_t mLength = 0;
    size_t mOffset = 0;
};
