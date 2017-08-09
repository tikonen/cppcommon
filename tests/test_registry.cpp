
#include <gtest/gtest.h>

#ifdef WIN32
#include <Windows.h>
#include <stdexcept>

#include <RegistryUtil.h>


TEST(Registry, Fail)
{
    bool fCaught = false;
    try {
        // should not be allowed to create key here
        RegistryUtil reg(L"Foo", HKEY_CLASSES_ROOT);
    } catch (const std::exception&) {
        fCaught = true;
    }
    ASSERT_TRUE(fCaught);
}

TEST(Registry, SetGet)
{
    RegistryUtil reg(L"Software\\Dev\\Tests");
    reg.SetValue(L"Test", 0xCAFEBABE);
    DWORD val;
    reg.GetValue(L"Test", &val);
    ASSERT_EQ(val, 0xCAFEBABE);
}

#endif // WIN32