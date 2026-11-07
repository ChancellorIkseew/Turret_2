// validator_test.cpp — tests for validator::to<T> string conversion
#include <limits>
#include "catch2/catch_amalgamated.hpp"
#include "engine/io/parser/validator.hpp"

TEST_CASE("validator::to parses unsigned integers", "[validator]") {
    SECTION("uint8_t") {
        REQUIRE(validator::to<uint8_t>("0").value() == 0);
        REQUIRE(validator::to<uint8_t>("100").value() == 100);
        REQUIRE(validator::to<uint8_t>("255").value() == 255);
        REQUIRE(validator::to<uint8_t>("256").value() == 255);
        REQUIRE(!validator::to<uint8_t>("abc").has_value());
        REQUIRE(!validator::to<uint8_t>("").has_value());
    }
    SECTION("uint16_t") {
        REQUIRE(validator::to<uint16_t>("0").value() == 0);
        REQUIRE(validator::to<uint16_t>("65535").value() == 65535);
        REQUIRE(validator::to<uint16_t>("65536").value() == 65535);
    }
    SECTION("uint32_t") {
        REQUIRE(validator::to<uint32_t>("0").value() == 0);
        REQUIRE(validator::to<uint32_t>("4294967295").value() == 4294967295u);
        REQUIRE(validator::to<uint32_t>("4294967296").value() == 4294967295u);
    }
    SECTION("uint64_t") {
        REQUIRE(validator::to<uint64_t>("0").value() == 0);
        REQUIRE(validator::to<uint64_t>("18446744073709551615").value() == UINT64_MAX);
    }
}

TEST_CASE("validator::to parses signed integers", "[validator]") {
    SECTION("int8_t") {
        REQUIRE(validator::to<int8_t>("0").value() == 0);
        REQUIRE(validator::to<int8_t>("127").value() == 127);
        REQUIRE(validator::to<int8_t>("-128").value() == -128);
        REQUIRE_FALSE(validator::to<int8_t>("128").has_value());
    }
    SECTION("int16_t") {
        REQUIRE(validator::to<int16_t>("0").value() == 0);
        REQUIRE(validator::to<int16_t>("32767").value() == 32767);
        REQUIRE(validator::to<int16_t>("-32768").value() == -32768);
    }
    SECTION("int32_t") {
        REQUIRE(validator::to<int32_t>("0").value() == 0);
        REQUIRE(validator::to<int32_t>("2147483647").value() == 2147483647);
        REQUIRE(validator::to<int32_t>("-2147483648").value() == INT32_MIN);
    }
    SECTION("int64_t") {
        REQUIRE(validator::to<int64_t>("0").value() == 0);
        REQUIRE(validator::to<int64_t>("9223372036854775807").value() == INT64_MAX);
        REQUIRE(validator::to<int64_t>("-9223372036854775808").value() == INT64_MIN);
    }
}

TEST_CASE("validator::to parses float", "[validator]") {
    REQUIRE(validator::to<float>("0").value() == 0.f);
    REQUIRE(validator::to<float>("1.5").value() == 1.5f);
    REQUIRE(validator::to<float>("-3.14").value() == -3.14f);
    REQUIRE_FALSE(validator::to<float>("abc").has_value());
}

TEST_CASE("validator::to parses bool", "[validator]") {
    REQUIRE(validator::to<bool>("true").value() == true);
    REQUIRE(validator::to<bool>("false").value() == false);
    REQUIRE_FALSE(validator::to<bool>("1").has_value());
    REQUIRE_FALSE(validator::to<bool>("yes").has_value());
}

TEST_CASE("validator::to parses string", "[validator]") {
    REQUIRE(validator::to<std::string>("hello").value() == "hello");
    REQUIRE(validator::to<std::string>("").value() == "");
}

TEST_CASE("validator::to parses TileCoord", "[validator]") {
    const auto tc = validator::to<TileCoord>("100|200").value();
    REQUIRE(tc.x == 100);
    REQUIRE(tc.y == 200);
    REQUIRE_FALSE(validator::to<TileCoord>("abc").has_value());
    REQUIRE_FALSE(validator::to<TileCoord>("100").has_value());
}

TEST_CASE("validator::to parses PixelCoord", "[validator]") {
    const auto pc = validator::to<PixelCoord>("1.5|2.5").value();
    REQUIRE(pc.x == 1.5f);
    REQUIRE(pc.y == 2.5f);
    REQUIRE_FALSE(validator::to<PixelCoord>("abc").has_value());
}

TEST_CASE("validator::to rejects garbage", "[validator]") {
    REQUIRE_FALSE(validator::to<int>("abc123").has_value());
    REQUIRE_FALSE(validator::to<int>("123abc").has_value());
    REQUIRE_FALSE(validator::to<int>("").has_value());
    REQUIRE_FALSE(validator::to<int>("  ").has_value());
    REQUIRE_FALSE(validator::to<int>("12 34").has_value());
}
