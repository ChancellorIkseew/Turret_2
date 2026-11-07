// hitbox_test.cpp — tests for RectHitbox and CircleHitbox
#include "catch2/catch_amalgamated.hpp"
#include "game/common/hitbox.hpp"

TEST_CASE("RectHitbox contains point", "[hitbox]") {
    RectHitbox hitbox(PixelCoord(10.f, 10.f), PixelCoord(20.f, 20.f));

    SECTION("inside") {
        REQUIRE(hitbox.contains(PixelCoord(15.f, 15.f)));
    }
    SECTION("on border") {
        REQUIRE(hitbox.contains(PixelCoord(10.f, 10.f)));
        REQUIRE(hitbox.contains(PixelCoord(20.f, 20.f)));
    }
    SECTION("outside") {
        REQUIRE_FALSE(hitbox.contains(PixelCoord(5.f, 15.f)));
        REQUIRE_FALSE(hitbox.contains(PixelCoord(25.f, 15.f)));
        REQUIRE_FALSE(hitbox.contains(PixelCoord(15.f, 5.f)));
        REQUIRE_FALSE(hitbox.contains(PixelCoord(15.f, 25.f)));
    }
}

TEST_CASE("RectHitbox intersects another RectHitbox", "[hitbox]") {
    RectHitbox hitbox1(PixelCoord(10.f, 10.f), PixelCoord(20.f, 20.f));

    SECTION("overlapping") {
        RectHitbox hitbox2(PixelCoord(15.f, 15.f), PixelCoord(25.f, 25.f));
        REQUIRE(hitbox1.intersects(hitbox2));
    }
    SECTION("non-overlapping") {
        RectHitbox hitbox3(PixelCoord(5.f, 5.f), PixelCoord(9.f, 9.f));
        REQUIRE_FALSE(hitbox1.intersects(hitbox3));

        RectHitbox hitbox4(PixelCoord(21.f, 21.f), PixelCoord(30.f, 30.f));
        REQUIRE_FALSE(hitbox1.intersects(hitbox4));
    }
    SECTION("point on corner") {
        RectHitbox point(PixelCoord(10.f, 10.f), PixelCoord(10.f, 10.f));
        REQUIRE(hitbox1.intersects(point));
        REQUIRE(point.intersects(hitbox1));
    }
}

TEST_CASE("RectHitbox overlap returns intersection size", "[hitbox]") {
    RectHitbox hitbox1(PixelCoord(10.f, 10.f), PixelCoord(20.f, 20.f));

    SECTION("partial overlap") {
        RectHitbox hitbox2(PixelCoord(15.f, 15.f), PixelCoord(25.f, 25.f));
        PixelCoord result = hitbox1.overlap(hitbox2);
        REQUIRE(result.x == Catch::Approx(5.f).margin(0.001f));
        REQUIRE(result.y == Catch::Approx(5.f).margin(0.001f));
    }
    SECTION("no overlap returns negative") {
        RectHitbox hitbox3(PixelCoord(5.f, 5.f), PixelCoord(9.f, 9.f));
        PixelCoord result = hitbox1.overlap(hitbox3);
        REQUIRE(result.x < 0.f);
        REQUIRE(result.y < 0.f);
    }
    SECTION("fully contained") {
        RectHitbox hitbox4(PixelCoord(12.f, 12.f), PixelCoord(18.f, 18.f));
        PixelCoord result = hitbox1.overlap(hitbox4);
        REQUIRE(result.x == Catch::Approx(6.f).margin(0.001f));
        REQUIRE(result.y == Catch::Approx(6.f).margin(0.001f));
    }
    SECTION("point on corner") {
        RectHitbox point(PixelCoord(10.f, 10.f), PixelCoord(10.f, 10.f));
        PixelCoord result = hitbox1.overlap(point);
        REQUIRE(result.x == Catch::Approx(0.f).margin(0.001f));
        REQUIRE(result.y == Catch::Approx(0.f).margin(0.001f));
    }
}

TEST_CASE("CircleHitbox contains point", "[hitbox]") {
    CircleHitbox circle(PixelCoord(10.f, 10.f), 5.f);

    SECTION("inside") {
        REQUIRE(circle.contains(PixelCoord(12.f, 12.f)));
    }
    SECTION("center") {
        REQUIRE(circle.contains(PixelCoord(10.f, 10.f)));
    }
    SECTION("outside") {
        REQUIRE_FALSE(circle.contains(PixelCoord(20.f, 20.f)));
    }
}

TEST_CASE("CircleHitbox intersects RectHitbox", "[hitbox]") {
    CircleHitbox circle(PixelCoord(15.f, 15.f), 5.f);
    RectHitbox rect(PixelCoord(10.f, 10.f), PixelCoord(20.f, 20.f));
    REQUIRE(circle.intersects(rect));

    RectHitbox farRect(PixelCoord(100.f, 100.f), PixelCoord(110.f, 110.f));
    REQUIRE_FALSE(circle.intersects(farRect));
}
