#include "pch.h"
#include "game/physics/id_manager.hpp"
#include "game/physics/mob_manager.hpp"

TEST(IDManagerTest, getNew_setFree) {
    IDManager<uint8_t> manager;
    EXPECT_TRUE(manager.getNext() == 0);
    EXPECT_TRUE(manager.getNext() == 1);
    EXPECT_TRUE(manager.getNext() == 2);
    EXPECT_TRUE(manager.getNext() == 3);

    manager.setFree(3);
    EXPECT_TRUE(manager.getNext() == 3);
}
/*
TEST(MobManagerTest, tt) {
    MobManager manager;



}
*/
