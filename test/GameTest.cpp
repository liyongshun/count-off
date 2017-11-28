#include "gtest/gtest.h"
#include "CountOffGame.h"

struct GameTest : testing::Test
{
    GameTest() : game_sprint_1_2({{3, "石头"},{5, "剪刀"},{7, "布"}})
               , game_sprint_3({{5, "石头"},{7, "剪刀"},{9, "布"}})
               , game_sprint_4({{3, "老虎"},{5, "棒子"},{7, "鸡"}})
    {
    }

    void ASSERT_COUNTOFF_3_5_7(int n, const std::string& words)
    {
        ASSERT_COUNTOFF(game_sprint_1_2, n, words);
    }

    void ASSERT_COUNTOFF_5_7_9(int n, const std::string& words)
    {
        ASSERT_COUNTOFF(game_sprint_3, n, words);
    }

    void ASSERT_COUNTOFF_3_5_7_EX(int n, const std::string& words)
    {
        ASSERT_COUNTOFF(game_sprint_4, n, words);
    }

private:
    void ASSERT_COUNTOFF(const CountOffGame& game, int n, const std::string& words)
    {
        ASSERT_EQ(words, game.countOff(n));
    }

protected:
    CountOffGame game_sprint_1_2;
    CountOffGame game_sprint_3;
    CountOffGame game_sprint_4;
};

TEST_F(GameTest, should_count_off_given_special_num_3_5_7)
{
    ASSERT_COUNTOFF_3_5_7(1,   "1");
    ASSERT_COUNTOFF_3_5_7(2,   "2");
    ASSERT_COUNTOFF_3_5_7(3,   "石头");
    ASSERT_COUNTOFF_3_5_7(5,   "剪刀");
    ASSERT_COUNTOFF_3_5_7(7,   "布");
    ASSERT_COUNTOFF_3_5_7(15,  "石头剪刀");
    ASSERT_COUNTOFF_3_5_7(21,  "石头布");
    ASSERT_COUNTOFF_3_5_7(105, "石头剪刀布");

    ASSERT_COUNTOFF_3_5_7(13,   "石头");
    ASSERT_COUNTOFF_3_5_7(35,   "石头");
}

TEST_F(GameTest, should_count_off_given_special_num_5_7_9)
{
    ASSERT_COUNTOFF_5_7_9(1,   "1");
    ASSERT_COUNTOFF_5_7_9(2,   "2");
    ASSERT_COUNTOFF_5_7_9(5,   "石头");
    ASSERT_COUNTOFF_5_7_9(7,   "剪刀");
    ASSERT_COUNTOFF_5_7_9(9,   "布");
    ASSERT_COUNTOFF_5_7_9(63,  "剪刀布");

    ASSERT_COUNTOFF_5_7_9(15,  "石头");
    ASSERT_COUNTOFF_5_7_9(35,  "石头");
}

TEST_F(GameTest, should_count_off_given_special_num_3_5_7_countof_other)
{
    ASSERT_COUNTOFF_3_5_7_EX(1,   "1");
    ASSERT_COUNTOFF_3_5_7_EX(2,   "2");
    ASSERT_COUNTOFF_3_5_7_EX(3,   "老虎");
    ASSERT_COUNTOFF_3_5_7_EX(5,   "棒子");
    ASSERT_COUNTOFF_3_5_7_EX(7,   "鸡");
    ASSERT_COUNTOFF_3_5_7_EX(15,  "老虎棒子");
    ASSERT_COUNTOFF_3_5_7_EX(21,  "老虎鸡");
    ASSERT_COUNTOFF_3_5_7_EX(105, "老虎棒子鸡");

    ASSERT_COUNTOFF_3_5_7_EX(13,  "老虎");
    ASSERT_COUNTOFF_3_5_7_EX(35,  "老虎");
}
