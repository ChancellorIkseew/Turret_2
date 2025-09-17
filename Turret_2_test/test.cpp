#include "pch.h"
#include "game/physics/physics_base.hpp"

TEST(HitboxTest, ContainsPixelCoord) {
    Hitbox hitbox(PixelCoord(10.f, 10.f), PixelCoord(20.f, 20.f));

    EXPECT_TRUE(hitbox.contains(PixelCoord(15.f, 15.f))); // Inter.
    EXPECT_TRUE(hitbox.contains(PixelCoord(10.f, 10.f))); // On border.
    EXPECT_TRUE(hitbox.contains(PixelCoord(20.f, 20.f))); // On border.

    EXPECT_FALSE(hitbox.contains(PixelCoord(5.f, 15.f)));  // Left.
    EXPECT_FALSE(hitbox.contains(PixelCoord(25.f, 15.f))); // Right.
    EXPECT_FALSE(hitbox.contains(PixelCoord(15.f, 5.f)));  // Up.
    EXPECT_FALSE(hitbox.contains(PixelCoord(15.f, 25.f))); // Down.
}

TEST(HitboxTest, IntersectsHitbox) {
    Hitbox hitbox1(PixelCoord(10.f, 10.f), PixelCoord(20.f, 20.f));
    Hitbox hitbox2(PixelCoord(15.f, 15.f), PixelCoord(25.f, 25.f)); // ������������
    Hitbox hitbox3(PixelCoord(5.f, 5.f),   PixelCoord(9.f, 9.f));   // �� ������������
    Hitbox hitbox4(PixelCoord(21.f, 21.f), PixelCoord(30.f, 30.f)); // �� ������������
    Hitbox hitbox5(PixelCoord(10.f, 10.f), PixelCoord(10.f, 10.f)); // ����� �� �������

    EXPECT_TRUE(hitbox1.intersects(hitbox2));
    EXPECT_FALSE(hitbox1.intersects(hitbox3));
    EXPECT_FALSE(hitbox1.intersects(hitbox4));
    EXPECT_TRUE(hitbox1.intersects(hitbox5));  // ����� �� ������� - ����������
    EXPECT_TRUE(hitbox5.intersects(hitbox1));  // ����� �� ������� - ����������
}

TEST(HitboxTest, OverlapHitbox) {
    Hitbox hitbox1(PixelCoord(10.f, 10.f), PixelCoord(20.f, 20.f));
    Hitbox hitbox2(PixelCoord(15.f, 15.f), PixelCoord(25.f, 25.f)); // ������������
    Hitbox hitbox3(PixelCoord(5.f, 5.f),   PixelCoord(9.f, 9.f));   // �� ������������
    Hitbox hitbox4(PixelCoord(12.f, 12.f), PixelCoord(18.f, 18.f)); // ������ hitbox1
    Hitbox hitbox5(PixelCoord(10.f, 10.f), PixelCoord(10.f, 10.f)); // ����� �� �������

    PixelCoord overlap1 = hitbox1.overlap(hitbox2);
    EXPECT_EQ(overlap1.x, 5); // ���������� 5 �� X
    EXPECT_EQ(overlap1.y, 5); // ���������� 5 �� Y

    PixelCoord overlap2 = hitbox1.overlap(hitbox3);
    EXPECT_EQ(overlap2.x, -1); // ��� ���������� (������ ���� 0, �� ��-�� min/max ���������� -1)
    EXPECT_EQ(overlap2.y, -1); // ��� ����������

    PixelCoord overlap3 = hitbox1.overlap(hitbox4);
    EXPECT_EQ(overlap3.x, 6);
    EXPECT_EQ(overlap3.y, 6);

    PixelCoord overlap4 = hitbox1.overlap(hitbox5);
    EXPECT_EQ(overlap4.x, 0);
    EXPECT_EQ(overlap4.y, 0);
}
