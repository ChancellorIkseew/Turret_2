#include "pch.h"
#ifdef FORM_VALIDATOR_TEST
#include "engine/io/utf8/utf8.hpp"
#include "engine/widgets/form_editor/form_validator.hpp"
#include "engine/widgets/form_editor/form_validator.cpp"
#include "engine/io/parser/validator.hpp"
#include "engine/io/parser/validator.cpp"
//TODO: find other way to link cpp files

TEST(NumberToU32StringConvertor, ToU32String) {
    EXPECT_TRUE(utf8::to_u32string(uint8_t(100U)) == U"100");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<uint8_t>::max()) == U"255");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<uint8_t>::min()) == U"0");
    EXPECT_TRUE(utf8::to_u32string(uint16_t(100U)) == U"100");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<uint16_t>::max()) == U"65535");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<uint16_t>::min()) == U"0");
    EXPECT_TRUE(utf8::to_u32string(uint32_t(100U)) == U"100");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<uint32_t>::max()) == U"4294967295");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<uint32_t>::min()) == U"0");
    EXPECT_TRUE(utf8::to_u32string(uint64_t(100U)) == U"100");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<uint64_t>::max()) == U"18446744073709551615");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<uint64_t>::min()) == U"0");

    EXPECT_TRUE(utf8::to_u32string(int8_t(100)) == U"100");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<int8_t>::max()) ==  U"127");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<int8_t>::min()) == U"-128");
    EXPECT_TRUE(utf8::to_u32string(int16_t(100)) == U"100");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<int16_t>::max()) ==  U"32767");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<int16_t>::min()) == U"-32768");
    EXPECT_TRUE(utf8::to_u32string(int32_t(100)) == U"100");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<int32_t>::max()) ==  U"2147483647");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<int32_t>::min()) == U"-2147483648");
    EXPECT_TRUE(utf8::to_u32string(int64_t(100)) == U"100");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<int64_t>::max()) ==  U"9223372036854775807");
    EXPECT_TRUE(utf8::to_u32string(std::numeric_limits<int64_t>::min()) == U"-9223372036854775808");
}

TEST(FormValidatorTest, ValidatedText) {
    std::u32string emptyString = U"";
    std::u32string someGarbage = U"DU6YXTY______+++++===76==fedfkdgkd89";
    std::u32string normalUint64 = U"500";
    std::u32string maxUint64 = utf8::to_u32string(std::numeric_limits<uint64_t>::max());
    std::u32string minUint64 = utf8::to_u32string(std::numeric_limits<uint64_t>::min());
    std::u32string normalInt64 = U"-500";

    Uint64Validator u64validator(0U, 0U);
    u64validator.validateText(emptyString);
    u64validator.validateText(someGarbage);
    u64validator.validateText(normalUint64);
    u64validator.validateText(normalInt64);

    EXPECT_TRUE(emptyString == U"");
    EXPECT_TRUE(someGarbage == U"67689");
    EXPECT_TRUE(normalUint64 == U"500");
    EXPECT_TRUE(maxUint64 == utf8::to_u32string(std::numeric_limits<uint64_t>::max()));
    EXPECT_TRUE(minUint64 == utf8::to_u32string(std::numeric_limits<uint64_t>::min()));
}

#endif
