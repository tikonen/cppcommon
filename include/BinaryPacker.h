#pragma once

#include <list>
#include <intrin.h>

class BinaryPacker {
public:
	enum Endianess {
		LITTLE_ENDIAN,
		BIG_ENDIAN
	};

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
		return add((BYTE)b);
	}

	BinaryPacker& add(const BYTE * pData, DWORD dwLen)
	{
		Append(Unit(dwLen, pData));
		return *this;
	}

	BinaryPacker& skip(DWORD dwLen)
	{
		return add(nullptr, dwLen);
	}

	BinaryPacker& set(DWORD dwLen, BYTE b)
	{
		skip(dwLen);
		// store the fill byte value
		mData.back().Data.sData = 0x00FF | (b << 8);
		return *this;
	}

	void CopyTo(BYTE *pDst, DWORD &dwLen)
	{
		DWORD dLen = 0;
		for (auto it = mData.cbegin(); it != mData.end(); it++) {
			auto& unit = *it;
			if (dLen + unit.len > dwLen) {
				break;
			}
			if (unit.pData) {
				memcpy(pDst + dLen, unit.pData, unit.len);
			} else {
				// skip
				if (unit.Data.sData & 0xFF) {
					// fill with a byte value
					BYTE b = (unit.Data.sData >> 8) & 0xFF;
					memset(pDst + dLen, b, unit.len);
				}
			}
			dLen += unit.len;
		}
		dwLen = dLen;
	}

	DWORD GetLength()
	{
		return mOffset;
	}
protected:

	struct Unit {
		DWORD len = 0;
		const BYTE * pData = (BYTE*)&Data;
		union {
			BYTE bData;
			DWORD dwData;
			SHORT sData;
		} Data{};

		Unit(const Unit& evlc) = delete;
		void operator=(const Unit& evlc) = delete;

		void Assign(Unit &u)
		{
			len = u.len;
			pData = u.pData;
			if (u.pData && len <= 4) {
				pData = (BYTE*)&Data;
				Data = u.Data;
			}
		}

		Unit(Unit&& u)
		{
			Assign(u);
		}
		Unit& operator=(Unit&& u)
		{
			Assign(u);
		}

		Unit(DWORD l, const BYTE * data)
		{
			len = l;
			pData = NULL;
			if (data) {
				if (len <= 4) {
					pData = (BYTE*)&Data;
					memcpy(&Data, data, len);
				} else {
					pData = data;
					Data.dwData = 0;
				}
			}
		}

		Unit(BYTE b)
		{
			len = 1;
			Data.bData = b;
		}

		Unit(DWORD w, Endianess end)
		{
			len = sizeof(w);
			Data.dwData = end == LITTLE_ENDIAN ? w : _byteswap_ulong(w);
		}

		Unit(WORD s, Endianess end)
		{
			len = sizeof(s);
			Data.sData = end == LITTLE_ENDIAN ? s : _byteswap_ushort(s);
		}
	};

	template <typename T>
	BinaryPacker &add(T u)
	{
		Append(Unit(u));
		return *this;
	};

	template <typename T>
	BinaryPacker &add(T u, Endianess end)
	{
		Append(Unit(u, end));
		return *this;
	};

	void Append(Unit &&unit)
	{
		mData.push_back(std::move(unit));
		mOffset += unit.len;
	}

private:
	std::list<Unit> mData;
	DWORD mOffset = 0;
};
