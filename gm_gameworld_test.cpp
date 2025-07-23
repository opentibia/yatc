#include <gtest/gtest.h>
#include "gm_gameworld.h"
#include "gamecontent/map.h"
#include "console.h"
#include "gamecontent/creature.h"
#include "gamecontent/globalvars.h"

// Test fixture for GM_Gameworld
class GM_GameworldTest : public ::testing::Test {
protected:
    GM_Gameworld* gameworld;

    void SetUp() override {
        gameworld = new GM_Gameworld();
    
        Objects::getInstance()->loadDat("Tibia.dat"); // TODO: should not try to present winOutfit_t and thus require loading outift and dat!
    }

    void TearDown() override {
        Objects::getInstance()->unloadDat();
        Objects::destroyInstance();
        // TODO: unload gfx?
        delete gameworld;
    }
};

// Test onCreatureSpeak with SPEAK_PRIVATE_NP
TEST_F(GM_GameworldTest, OnCreatureSpeak_PrivateNP) {
    std::string name = "NPC";
    std::string message = "Hello, player!";
    Position pos = {100, 100, 7};

    gameworld->onCreatureSpeak(SPEAK_PRIVATE_NP, 0, name, 0, pos, message);

    auto console = gameworld->findConsole("NPCs");
    ASSERT_NE(console, nullptr);
    const auto& lastEntry = console->getLastEntry();
    EXPECT_EQ(lastEntry.getFullText(), message);
    EXPECT_EQ(lastEntry.getSpeaker(), name);
    EXPECT_EQ(lastEntry.getColor(), TEXTCOLOR_LIGHTBLUE);
}

// Test onCreatureSpeak with SPEAK_MONSTER_SAY
TEST_F(GM_GameworldTest, OnCreatureSpeak_MonsterSay) {
    std::string name = "Monster";
    std::string message = "Roar!";
    Position pos = {100, 100, 7};

    gameworld->onCreatureSpeak(SPEAK_MONSTER_SAY, 0, name, 0, pos, message);

    auto map = Map::getInstance();
    const auto& publicMessage = map.getPublicMessages().back();
    EXPECT_EQ(publicMessage.getText(), message);
    EXPECT_EQ(publicMessage.getSender(), name);
    EXPECT_EQ(publicMessage.getColor(), TEXTCOLOR_ORANGE);
}

// Test onCreatureSpeak with SPEAK_WHISPER
TEST_F(GM_GameworldTest, OnCreatureSpeak_Whisper) {
    std::string name = "Player";
    std::string message = "Psst!";
    Position pos = {100, 100, 7};

    gameworld->onCreatureSpeak(SPEAK_WHISPER, 0, name, 0, pos, message);

    auto console = gameworld->getDefaultConsole();
    ASSERT_NE(console, nullptr);
    const auto& lastEntry = console->getLastEntry();
    EXPECT_EQ(lastEntry.getFullText(), message);
    EXPECT_EQ(lastEntry.getSpeaker(), name);
    EXPECT_EQ(lastEntry.getColor(), TEXTCOLOR_YELLOW);
}

// Test onCreatureSpeak with SPEAK_SAY
TEST_F(GM_GameworldTest, OnCreatureSpeak_Say) {
    std::string name = "Player";
    std::string message = "Hello!";
    Position pos = {100, 100, 7};

    gameworld->onCreatureSpeak(SPEAK_SAY, 0, name, 0, pos, message);

    auto console = gameworld->getDefaultConsole();
    ASSERT_NE(console, nullptr);
    const auto& lastEntry = console->getLastEntry();
    EXPECT_EQ(lastEntry.getFullText(), message);
    EXPECT_EQ(lastEntry.getSpeaker(), name);
    EXPECT_EQ(lastEntry.getColor(), TEXTCOLOR_YELLOW);
}

// Test onCreatureSpeak with SPEAK_YELL
TEST_F(GM_GameworldTest, OnCreatureSpeak_Yell) {
    std::string name = "Player";
    std::string message = "Hey!";
    Position pos = {100, 100, 7};

    gameworld->onCreatureSpeak(SPEAK_YELL, 0, name, 0, pos, message);

    auto console = gameworld->getDefaultConsole();
    ASSERT_NE(console, nullptr);
    const auto& lastEntry = console->getLastEntry();
    EXPECT_EQ(lastEntry.getFullText(), message);
    EXPECT_EQ(lastEntry.getSpeaker(), name);
    EXPECT_EQ(lastEntry.getColor(), TEXTCOLOR_YELLOW);
}

// Test onCreatureSpeak with SPEAK_PRIVATE
TEST_F(GM_GameworldTest, OnCreatureSpeak_Private) {
    std::string name = "Player";
    std::string message = "Private message";
    Position pos = {100, 100, 7};

    gameworld->onCreatureSpeak(SPEAK_PRIVATE, 0, name, 0, pos, message);

    auto console = gameworld->findConsole(name);
    ASSERT_NE(console, nullptr);
    const auto& lastEntry = console->getLastEntry();
    EXPECT_EQ(lastEntry.getFullText(), message);
    EXPECT_EQ(lastEntry.getSpeaker(), name);
    EXPECT_EQ(lastEntry.getColor(), TEXTCOLOR_LIGHTBLUE);
}