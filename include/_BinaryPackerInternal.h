#pragma once

#include <Endianess.h>

#include <stdint.h>

namespace _BinaryPackerInternal
{
    union Data {
        uint8_t bData;
        uint32_t dwData;
        uint16_t wData;
        uint64_t qwData;
    };

    template <int> struct int_functor;

    template <> struct int_functor<1> {
        uint8_t operator()(Data* d, bool) const { return d->bData; }
        void operator()(uint8_t b, Data* d, bool) const { d->bData = b; }
    };

    template <> struct int_functor<2> {
        uint16_t operator()(Data* d, bool le) const
        {
            return le ? le16toh(d->wData) : be16toh(d->wData);
        }
        void operator()(uint16_t w, Data* d, bool le) const { d->wData = le ? htole16(w) : htobe16(w); }
    };


    template <> struct int_functor<4> {
        uint32_t operator()(Data* d, bool le) const
        {
            return le ? le32toh(d->dwData) : be32toh(d->dwData);
        }
        void operator()(uint32_t dw, Data* d, bool le) const
        {
            d->dwData = le ? htole32(dw) : htobe32(dw);
        }
    };

    template <> struct int_functor<8> {
        uint64_t operator()(Data* d, bool le) const
        {
            return le ? le64toh(d->qwData) : be64toh(d->qwData);
        }
        void operator()(uint64_t qw, Data* d, bool le) const
        {
            d->qwData = le ? htole64(qw) : htobe64(qw);
        }
    };
} // namespace _BinaryPackerInternal
