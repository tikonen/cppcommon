
#include <gtest/gtest.h>

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
    BYTE data[128];
    int l = sizeof(data);
    pack.CopyTo(data, l);
    BYTE p[] = { 0x1, 0x0, 0x0, 0x3 };
    ASSERT_FALSE(memcmp(data, p, l));

    BinaryUnPacker unpack(data, l);
    BYTE b;
    unpack.readByte(&b);
    ASSERT_EQ(1, b);
    unpack.skip(2);
    unpack.readByte(&b);
    ASSERT_EQ(3, b);
}

TEST(BinaryPacker, Array)
{
    BinaryPacker pack;
    pack.add((BYTE*)"kissa", 6);
    ASSERT_EQ(6, pack.GetLength());
    BYTE data[128];
    int l = sizeof(data);
    pack.CopyTo(data, l);
    ASSERT_STREQ("kissa", (char*)data);

    BYTE str[6];
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
    ASSERT_EQ(sizeof(WORD) * 2 + sizeof(DWORD) * 2, pack.GetLength());
    BYTE data[128];
    BYTE* ptr = data;
    int l = sizeof(data);
    pack.CopyTo(data, l);

    ASSERT_EQ(0xABCD, *(WORD*)ptr);
    ptr += sizeof(WORD);
    ASSERT_EQ(0xCDAB, *(WORD*)ptr);
    ptr += sizeof(WORD);
    ASSERT_EQ(0xCAFEBABE, *(DWORD*)ptr);
    ptr += sizeof(DWORD);
    ASSERT_EQ(0xBEBAFECA, *(DWORD*)ptr);

    BinaryUnPacker unpack(data, l);
    WORD w1, w2;
    DWORD dw1, dw2;
    unpack.read16bWord(&w1, LITTLE_ENDIAN)
    .read16bWord(&w2, BIG_ENDIAN)
    .read32bWord(&dw1, LITTLE_ENDIAN)
    .read32bWord(&dw2, BIG_ENDIAN);

    ASSERT_EQ(0xABCD, w1);
    ASSERT_EQ(0xABCD, w2);
    ASSERT_EQ(0xCAFEBABE, dw1);
    ASSERT_EQ(0xCAFEBABE, dw2);
}
