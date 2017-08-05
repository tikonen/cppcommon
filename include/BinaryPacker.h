#pragma once

#include "_BinaryPackerInternal.h"

#include <vector>

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
