// inplace_vector_test.cpp — tests for custom InplaceVector (replace with std::inplace_vector in C++26)
#include <catch2/catch_test_macros.hpp>
#include "engine/custom_stl/inplace_vector.hpp"

struct TestItem {
    int a;
    float b;
};

static_assert(Trivial<TestItem>, "TestItem should satisfy Trivial concept");

TEST_CASE("InplaceVector default construction", "[inplace_vector]") {
    InplaceVector<int, 4> v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.empty());
    REQUIRE_FALSE(v.full());
    REQUIRE(v.capacity() == 4);
}

TEST_CASE("InplaceVector push_back", "[inplace_vector]") {
    InplaceVector<int, 4> v;
    REQUIRE(v.push_back(1));
    REQUIRE(v.push_back(2));
    REQUIRE(v.push_back(3));
    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v.push_back(4));
    REQUIRE(v.full());
    REQUIRE_FALSE(v.push_back(5)); // overflow, silently dropped
    REQUIRE(v.size() == 4);
}

TEST_CASE("InplaceVector pop_back", "[inplace_vector]") {
    InplaceVector<int, 4> v;
    v.push_back(10);
    v.push_back(20);
    v.pop_back();
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 10);
    v.pop_back();
    REQUIRE(v.empty());
    v.pop_back(); // pop from empty — no-op
    REQUIRE(v.empty());
}

TEST_CASE("InplaceVector clear", "[inplace_vector]") {
    InplaceVector<int, 4> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.clear();
    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
}

TEST_CASE("InplaceVector iteration", "[inplace_vector]") {
    InplaceVector<int, 4> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    int sum = 0;
    for (int x : v)
        sum += x;
    REQUIRE(sum == 60);
}

TEST_CASE("InplaceVector with custom type", "[inplace_vector]") {
    InplaceVector<TestItem, 3> v;
    v.push_back({1, 1.5f});
    v.push_back({2, 2.5f});
    REQUIRE(v.size() == 2);
    REQUIRE(v[0].a == 1);
    REQUIRE(v[1].b == 2.5f);
}

TEST_CASE("InplaceVector data pointer", "[inplace_vector]") {
    InplaceVector<int, 4> v;
    v.push_back(42);
    int* ptr = v.data();
    REQUIRE(*ptr == 42);
    const InplaceVector<int, 4>& cv = v;
    const int* cptr = cv.data();
    REQUIRE(*cptr == 42);
}
