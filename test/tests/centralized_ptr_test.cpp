#include "pch.h"
#include "../libs/include/CUSTOM_SMART_POINTERS/centralized_ptr.hpp"

struct DestructDetector {
    bool* destroyed;
    DestructDetector(bool* d) : destroyed(d) { *destroyed = false; }
    ~DestructDetector() { *destroyed = true; }
    void doSomething() {}
};

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) { }
};

struct Base {
    Base() = default;
    virtual ~Base() = default;
};

struct Derived : Base {
    Derived() = default;
    virtual ~Derived() override final = default;
};

TEST(CentralizedPtrTest, LifeCycle) {
    bool isDeleted = false;
    {
        csp::centralized_ptr<DestructDetector> ptr1(new DestructDetector(&isDeleted));
        {
            csp::centralized_ptr<DestructDetector> ptr2 = ptr1; // Копирование
            EXPECT_FALSE(isDeleted);
        } // ptr2 вышел из области видимости
        EXPECT_FALSE(isDeleted); // Объект должен быть жив, так как ptr1 его держит
    } // ptr1 вышел из области видимости
    EXPECT_TRUE(isDeleted); // Теперь удален
}

TEST(CentralizedPtrTest, Move) {
    bool isDeleted = false;
    {
        csp::centralized_ptr<DestructDetector> ptr1(new DestructDetector(&isDeleted));
        csp::centralized_ptr<DestructDetector> ptr2 = std::move(ptr1);
        EXPECT_FALSE(ptr1.operator bool()); // Этот обнулился
        EXPECT_DEATH({
            auto* p = ptr1.get();
            printf("%p", (void*)p);
            }, "");
        EXPECT_TRUE(ptr2.operator bool());
        EXPECT_TRUE(ptr2.get() != nullptr); // Новый владеет
        EXPECT_TRUE(isDeleted == false);    // ptr2 is valid
    }
    EXPECT_TRUE(isDeleted);
}

TEST(CentralizedPtrTest, CustomFree) {
    bool lambdaCalled = false;
    {
        csp::centralized_ptr<int> ptr(new int(5), [&]() {
            lambdaCalled = true;
            });
    }
    EXPECT_TRUE(lambdaCalled);
}

TEST(CentralizedPtrTest, MakeCentralized) {
    {
        csp::centralized_ptr<Derived> derivedPtr(new Derived());
        csp::centralized_ptr<Base> basePtr = csp::make_centralized<Derived>();
        EXPECT_TRUE(derivedPtr.operator bool());
        EXPECT_TRUE(basePtr.operator bool());
    }
    
    EXPECT_TRUE(true);
    //auto point = csp::make_centralized<Point>(2, 3);
    //EXPECT_TRUE(point->x == 2);
    //EXPECT_TRUE(point->y == 3);
}
