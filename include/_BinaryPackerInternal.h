#pragma once

#include <Endianess.h>

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
        BYTE operator()(Data* d, bool) const { return d->bData; }
        void operator()(BYTE b, Data* d, bool) const { d->bData = b; }
    };

    template <> struct int_functor<2> {
        WORD operator()(Data* d, bool le) const
        {
            return le ? le16toh(d->wData) : be16toh(d->wData);
        }
        void operator()(WORD w, Data* d, bool le) const { d->wData = le ? htole16(w) : htobe16(w); }
    };


    template <> struct int_functor<4> {
        DWORD operator()(Data* d, bool le) const
        {
            return le ? le32toh(d->dwData) : be32toh(d->dwData);
        }
        void operator()(DWORD dw, Data* d, bool le) const
        {
            d->dwData = le ? htole32(dw) : htobe32(dw);
        }
    };

    template <> struct int_functor<8> {
        LONG64 operator()(Data* d, bool le) const
        {
            return le ? le64toh(d->qwData) : be64toh(d->qwData);
        }
        void operator()(LONG64 qw, Data* d, bool le) const
        {
            d->qwData = le ? htole64(qw) : htobe64(qw);
        }
    };
} // namespace _BinaryPackerInternal
