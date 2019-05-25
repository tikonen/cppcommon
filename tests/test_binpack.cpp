
#include <gtest/gtest.h>

#ifdef WIN32
#include <Windows.h>
#endif

#include <BinaryPacker.h>
#include <BinaryUnPacker.h>


TEST(BinaryPacker, Zero)
{
    BinaryPacker pack;
    ASSERT_EQ(0, pack.GetLength());
    BinaryUnPacker unpack(NULL, 0);
}

TEST(BinaryPacker, Packet)
{
    BinaryPacker pack;
    pack.addByte(1).skip(2).addByte(3);

    ASSERT_EQ(4, pack.GetLength());
    uint8_t data[128];
    size_t l = sizeof(data);
    pack.CopyTo(data, l);
    uint8_t p[] = { 0x1, 0x0, 0x0, 0x3 };
    ASSERT_FALSE(memcmp(data, p, l));

    BinaryUnPacker unpack(data, l);
    uint8_t b;
    unpack.readByte(&b);
    ASSERT_EQ(1, b);
    unpack.skip(2);
    unpack.readByte(&b);
    ASSERT_EQ(3, b);
}

TEST(BinaryPacker, Array)
{
    BinaryPacker pack;
    pack.add((uint8_t*)"kissa", 6);
    ASSERT_EQ(6, pack.GetLength());
    uint8_t data[128];
    size_t l = sizeof(data);
    pack.CopyTo(data, l);
    ASSERT_STREQ("kissa", (char*)data);

    uint8_t str[6];
    BinaryUnPacker unpack(data, l);
    unpack.read(str, 6);
    ASSERT_STREQ("kissa", (char*)str);
}

TEST(BinaryPacker, WordEndianess)
{
    BinaryPacker pack;
    pack.add16bWord(0xABCD, LITTLE_ENDIAN)
        .add16bWord(0xABCD, BIG_ENDIAN)
        .add32bWord(0xCAFEBABE, LITTLE_ENDIAN)
        .add32bWord(0xCAFEBABE, BIG_ENDIAN);
    ASSERT_EQ(sizeof(uint16_t) * 2 + sizeof(uint32_t) * 2, pack.GetLength());
    uint8_t data[128];
    uint8_t* ptr = data;
    size_t l = sizeof(data);
    pack.CopyTo(data, l);

    ASSERT_EQ(0xABCD, *(uint16_t*)ptr);
    ptr += sizeof(uint16_t);
    ASSERT_EQ(0xCDAB, *(uint16_t*)ptr);
    ptr += sizeof(uint16_t);
    ASSERT_EQ(0xCAFEBABE, *(uint32_t*)ptr);
    ptr += sizeof(uint32_t);
    ASSERT_EQ(0xBEBAFECA, *(uint32_t*)ptr);

    BinaryUnPacker unpack(data, l);
    uint16_t w1, w2;
    uint32_t dw1, dw2;
    unpack.read16bWord(&w1, LITTLE_ENDIAN)
        .read16bWord(&w2, BIG_ENDIAN)
        .read32bWord(&dw1, LITTLE_ENDIAN)
        .read32bWord(&dw2, BIG_ENDIAN);

    ASSERT_EQ(0xABCD, w1);
    ASSERT_EQ(0xABCD, w2);
    ASSERT_EQ(0xCAFEBABE, dw1);
    ASSERT_EQ(0xCAFEBABE, dw2);
}

TEST(BinaryPacker, Invalid)
{
    BinaryUnPacker unpack(NULL, 0);
    bool fCaught = false;
    try {
        uint8_t b;
        unpack.readByte(&b);
    } catch (const std::exception&) {
        fCaught = true;
    }
    ASSERT_TRUE(fCaught);
}
