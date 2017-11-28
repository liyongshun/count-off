注：手机推荐横屏观看:-)

>幼儿园老师在给一群小朋友玩报数游戏，游戏规则如下：
>+ 老师给定任意三个特殊个位数：3,5,7
>+ 总共120位小朋友排成一排顺序报数

> ---
> 需求1：
a. 所报数字是第一个特殊数的倍数(本例为3)，不能说该数字，说“石头”；
b. 所报数字是第二个特殊数的倍数(本例为5)，不能说该数字，说“剪刀”；
c. 所报数字是第三个特殊数的倍数(本例为7)，不能说该数字，说“布”；
d. 如果所报数字同时是两个特殊数的倍数情况下，也要特殊处理，比如是3和5的倍数，那么不能说该数字，而是要说“石头剪刀”, 以此类推。
e. 如果同时是三个特殊数的倍数，那么要说“石头剪刀布”

## Sprint 1
快速浏览题目，从中识别出关键字“报数游戏”，“特殊数：3，5，7”，“120，顺序报数”。采用TDD方式，先驱动出接口。
第一个测试用例：
```cpp
#include "gtest/gtest.h"
#include "CountOffGame.h"

struct GameTest : testing::Test
{
protected:
    CountOffGame game;
};

TEST_F(GameTest, should_count_off_given_special_num_3_5_7)
{
    ASSERT_EQ("1", game.shout(1));
}
```

从解决编译问题开始，快速通过测试：

```cpp
//CountOffGame.h
#include <string>

struct CountOffGame
{
    std::string shout(int n) const;
};
```

```cpp
//CountOffGame.cpp
#include "CountOffGame.h"

std::string CountOffGame::shout(int n) const
{
    return "1";
}
```
至此，我们已经驱动出用户接口，通过第一个用例。

接下来，继续增加第二个用例:
> 注：**简单起见，本例不对测试用例进行拆分，请大家按照 F.I.R.S.T. 原则及 given-when-then方式自行编写测试用例 :-)**

```cpp
TEST_F(GameTest, should_count_off_given_special_num_3_5_7)
{
    ASSERT_EQ("1", game.shout(1));
    ASSERT_EQ("2", game.shout(2));
}
```

修改实现，老老实实把数字转换为字符串，通过第二个用例。

```cpp
//CountOffGame.cpp
#include "CountOffGame.h"

std::string CountOffGame::shout(int n) const
{
    return std::to_string(n);
}
```

接下来我们该处理需求1-a了:
>所报数字是第一个特殊数的倍数(本例为3)，不能说该数字，说“石头”

```cpp
TEST_F(GameTest, should_count_off_given_special_num_3_5_7)
{
    ASSERT_EQ("1", game.shout(1));
    ASSERT_EQ("2", game.shout(2));
    ASSERT_EQ("石头", game.shout(3));
}
```

运行测试，校验失败，继续完成需求1-a。这个难不倒我们，求某个数的倍数，用`%`即可。

```cpp
std::string CountOffGame::shout(int n) const
{
    if(n % 3 == 0) return "石头";
    return std::to_string(n);
}
```
继续完成需求1-b：
```cpp
TEST_F(GameTest, should_count_off_given_special_num_3_5_7)
{
    ASSERT_EQ("1", game.shout(1));
    ASSERT_EQ("2", game.shout(2));
    ASSERT_EQ("石头", game.shout(3));
    ASSERT_EQ("剪刀", game.shout(5));
}
```
```cpp
std::string CountOffGame::shout(int n) const
{
    if(n % 3 == 0) return "石头";
    if(n % 5 == 0) return "剪刀";
    return std::to_string(n);
}
```

至此，我们已经完成了需求1-a，需求1-b，但敏锐的你一定发现了不少坏味道：
1. 业务代码中需求1-a，需求1-b的实现存在明显重复
2. 测试用例中，`game.shout(...)`存在明显重复
3. 用户API`shout()`命名也不太合理

我们先忍忍，继续完成需求1-c，因为它并不会带来新的变化方向

```cpp
TEST_F(GameTest, should_count_off_given_special_num_3_5_7)
{
    ASSERT_EQ("1", game.shout(1));
    ASSERT_EQ("2", game.shout(2));
    ASSERT_EQ("石头", game.shout(3));
    ASSERT_EQ("剪刀", game.shout(5));
    ASSERT_EQ("布", game.shout(7));
}
```
```cpp
std::string CountOffGame::shout(int n) const
{
    if(n % 3 == 0) return "石头";
    if(n % 5 == 0) return "剪刀";
    if(n % 7 == 0) return "布";
    return std::to_string(n);
}
```

---

至此，我们已经完成需求1-a,b,c，接下来要开启`Refactor`模式了

+ 重命名`shout(int n)`接口

```cpp
//CountOffGame.h
struct CountOffGame
{
    std::string countOff(int n) const;
};

//CountOffGame.cpp
std::string CountOffGame::countOff(int n) const
{
   ...
}
```

+ 消除测试用例接口重复

```cpp
struct GameTest : testing::Test
{
    void ASSERT_COUNTOFF_GAME(int n, const std::string& words)
    {
        ASSERT_EQ(words, game.countOff(n));
    }

protected:
    CountOffGame game;
};

TEST_F(GameTest, should_count_off_given_special_num_3_5_7)
{
    ASSERT_COUNTOFF_GAME(1, "1");
    ASSERT_COUNTOFF_GAME(2, "2");
    ASSERT_COUNTOFF_GAME(3, "石头");
    ASSERT_COUNTOFF_GAME(5, "剪刀");
    ASSERT_COUNTOFF_GAME(7, "布");
}
```

+ 消除业务代码中重复，提炼出倍数概念

```cpp
namespace
{
    bool times(int n, int times)
    {
        return n % times == 0;
    }
}

std::string CountOffGame::countOff(int n) const
{
    if(times(n, 3)) return "石头";
    if(times(n, 5)) return "剪刀";
    if(times(n, 7)) return "布";
    return std::to_string(n);
}
```

+ 消除报数“石头，剪刀，布”硬编码

```cpp
namespace
{
    ...

    std::string shout(int n, const std::string& words = "")
    {
        if(!words.empty()) return words;
        return std::to_string(n);
    }
}

std::string CountOffGame::countOff(int n) const
{
    if(times(n, 3)) return shout(n, "石头");
    if(times(n, 5)) return shout(n, "剪刀");
    if(times(n, 7)) return shout(n, "布");
    return shout(n);
}
```

至此，我们已经消除了明显的重复，但是依然存在结构性重复。即每个语句都是报数时遵循的一个规则，并且都是`if(谓词) return 动作`结构。
我们对其进行抽象，进一步分析发现满足如下形式化定义：

```cpp
Rule: (int) -> string
Predicate: (int) -> bool
Action: (int) -> string
```

我们从武器库中快速搜寻着各种解决方案，比较着他们的利弊。

1. 抽象出`Predicate`与`Action`接口类，分别提供`bool isMatch(int n) const`与`std::string do(int n) const`虚方法；抽象出`Rule`类，注入`Predicate`与`Action`接口
2. 定义类模板`Rule`，绑定谓词与动作，要求谓词满足`isMatch(int n)`，动作满足`do(int n)`约束
3. 定义两个函数指针，用于抽象约束关系，使用函数模板`rule`将其绑定

综合考虑后，发现方案3在简单性和扩展性方面是最合适的
```cpp
//CountOffGame.cpp
namespace
{
    ...
    typedef bool (*Predicate)(int, int);
    typedef std::string (*Action)(int, const std::string& );

    template<Predicate isMatch, Action do>
    std::string rule(int n, int times, const std::string& words)
    {
        if(isMatch(n, times))
        {
            return do(n, words);
        }

        return std::string("");
    }
}

std::string CountOffGame::countOff(int n) const
{
    const std::string& r1_1 = rule<times, shout>(n, 3, "石头");
    if( ! r1_1.empty()) return r1_1;
    const std::string& r1_2 = rule<times, shout>(n, 5, "剪刀");
    if( ! r1_2.empty()) return r1_2;
    const std::string& r1_3 = rule<times, shout>(n, 7, "布");
    if( ! r1_3.empty()) return r1_3;

    return shout(n);
}
```
旧仇刚报，又添新恨，此时又出现了新的结构性重复，抽象一下就是：报数规则有一个满足，就执行该规则并返回，否则继续执行下一个规则，语义为为`anyof(...)`。我们暂且忍受一下，让子弹飞一会，继续完成下面需求。
增加需求1-d,需求1-e测试用例：

```cpp
TEST_F(GameTest, should_count_off_given_special_num_3_5_7)
{
    ASSERT_COUNTOFF_GAME(1,   "1");
    ASSERT_COUNTOFF_GAME(2,   "2");
    ASSERT_COUNTOFF_GAME(3,   "石头");
    ASSERT_COUNTOFF_GAME(5,   "剪刀");
    ASSERT_COUNTOFF_GAME(7,   "布");
    ASSERT_COUNTOFF_GAME(15,  "石头剪刀");
    ASSERT_COUNTOFF_GAME(21,  "石头布");
    ASSERT_COUNTOFF_GAME(35,  "剪刀布");
    ASSERT_COUNTOFF_GAME(105, "石头剪刀布");
}
```
我们仔细分析发现需求1-d,需求1-e是对上面需求的组合，即如果规则满足则执行该规则，完成后继续执行下一个规则，若不满足则直接执行下一个规则。语义为allof(...)。本例中表现为把所有规则结果串起来。

```cpp
std::string CountOffGame::countOff(int n) const
{
    const std::string& r1 = rule<times, shout>(n, 3, "石头") ;
                          + rule<times, shout>(n, 5, "剪刀");
                          + rule<times, shout>(n, 7, "布");
    if( ! r1.empty()) return r1;

    return shout(n);
}
```

至此，我们全部完成了需求1所有规则，由于需求1-d,e比需求1-a,b,c优先级高，allof()隐式满足了anyof()需求，所以结构性重复暂时不存在了。

---
## Sprint 2

>需求 2:
>a. 如果所报数字中包含了第一个特殊数，那么也不能说该数字，而是要说相应的词，比如本例中第一个特殊数是3，那么要报13的同学应该说“石头”。
b. 如果所报数字中包含了第一个特殊数，那么忽略规则1，比如要报35的同学只报“石头”，不报“剪刀布”

增加需求2测试用例：

```cpp
TEST_F(GameTest, should_count_off_given_special_num_3_5_7)
{
    ...

    ASSERT_COUNTOFF_GAME(13,   "石头");
    ASSERT_COUNTOFF_GAME(35,   "石头");
}
```
有了需求1的抽象，需求2就比较简单了，仅需要增加一个谓词即可

```cpp
namespace
{
    ...
    bool contains(int n, int contained)
    {
        int units = 0;
        while(n > 0)
        {
            units = n % 10;
            n = n /10;
            if(units == contained) return true;
        }

        return false;
    }
    ...
}

std::string CountOffGame::countOff(int n) const
{
    const std::string& r2 = rule<contains, shout>(n, 3, "石头");
    if(!r2.empty()) return r2;

    const std::string& r1 = rule<times, shout>(n, 3, "石头");
                          + rule<times, shout>(n, 5, "剪刀");
                          + rule<times, shout>(n, 7, "布");
    if( ! r1.empty()) return r1;

    return shout(n);
}
```

`return shout(n);`是什么鬼，我们进一步分析，将它抽象成一个规则

```cpp
namespace
{
    ...
    bool always_true(int,int)
    {
        return true;
    }

    std::string shout(int, const std::string& words)
    {
        return words;
    }

    std::string nop(int n, const std::string&)
    {
        return std::to_string(n);
    }
    ...
}

std::string CountOffGame::countOff(int n) const
{
    const std::string& r2 = rule<contains, shout>(n, 3, "石头");
    if(!r2.empty()) return r2;

    const std::string& r1 = rule<times, shout>(n, 3, "石头");
                          + rule<times, shout>(n, 5, "剪刀");
                          + rule<times, shout>(n, 7, "布");
    if( ! r1.empty()) return r1;

    return rule<always_true, nop>(n, 0, "");
}
```

但是我们讨厌的结构性重复又来了，这不就是前面抽象的`anyof(...)`么，看来不得不收拾他们了。
解决这个问题之前，我们先把物理耦合处理下，`CountOffGame.cpp`文件的职责已经太多了

```cpp
//Predicate.h
bool times(int n, int times);
bool contains(int n, int contained);
bool always_true(int,int);

//Predicate.cpp
#include "Predicate.h"

bool times(int n, int times)
{
    return n % times == 0;
}

bool contains(int n, int contained)
{
    int units = 0;
    while(n > 0)
    {
        units = n % 10;
        n = n /10;
        if(units == contained) return true;
    }

    return false;
}

bool always_true(int,int)
{
    return true;
}
```

```cpp
//Action.h
#include <string>

std::string shout(int n, const std::string& words);
std::string nop(int n, const std::string&);

//Action.cpp
#include "Action.h"

std::string shout(int, const std::string& words)
{
    return words;
}

std::string nop(int n, const std::string&)
{
    return std::to_string(n);
}
```

```cpp
//Rule.h
#include <string>

typedef bool (*Predicate)(int, int);
typedef std::string (*Action)(int, const std::string& );

template<Predicate isMatch, Action do>
std::string rule(int n, int times, const std::string& words)
{
    if(isMatch(n, times))
    {
        return do(n, words);
    }

    return std::string("");
}
```

`CountOffGame.cpp`文件中仅剩下组合游戏规则，完成游戏了

```cpp
std::string CountOffGame::countOff(int n) const
{
    const std::string& r2 = rule<contains, shout>(n, 3, "石头");
    if(!r2.empty()) return r2;

    const std::string& r1 = rule<times, shout>(n, 3, "石头");
                          + rule<times, shout>(n, 5, "剪刀");
                          + rule<times, shout>(n, 7, "布");
    if( ! r1.empty()) return r1;

    return shout(n);
}
```

> 注：**有同学不知道CountOffGame类与Rule的区别，这里解释一下，规则仅完成每个规则自己的形式化与定义，游戏中对规则进行组合，完成游戏**

完成文件物理解耦后，世界一下子清静多了，需要特别指出的是，在使用c语言进行编程时，由于没有类的模块化手段，在解耦方面，文件是留给我们的唯一利器了。

我们回过头来，再看看前面抽象的`anyof(...),allof(...)`概念，形式化一下可以表示为：

```cpp
Rule: (int) -> string
allof: rule1 && rule2 ...
anyof: rule1 || rule2 ...
```
这显然是规则的组合关系管理，我们又陷入了深深的思索，使用什么方法解决这个问题呢?
继续搜索武器库：

1. 使用面向对象方法，抽象出`rule`对应接口，使用组合模式解决该问题
2. 使用类模板，将不同`rule`通过模板参数注入，需要使用变参模板
3. 使用函数模板，将不同`rule`通过模板参数注入，需要使用变参函数模板
4. 使用函数式编程，可以使用`std::function`定义每个`rule`

综合比较以上方案，
+ 面向对象设计，组合模式中集合类要求每个成员`rule`不可以异构，并且仅能存放指向`rule`的指针，即要求每个`rule`都是一个对象。我们就需要管理每个对象的生命周期，需要使用`shared_ptr`,或者使用`c++11` `std::move`语义，延长临时对象生命周期。越想越复杂，打住！
+ 模板元编程，需要使用`c++11`变参模板，或者使用repeate宏了(参看boost库)，
+ 函数模板，需要使用`c++11`变参函数模板，使用尾递归完成组合，直接注入规则即可
+ 函数式编程，形式化表示规则，天生具有优势，又加之函数的无状态，组合起来很方便，可以结合右值引用及`std::move`完成，考虑熟悉同学较少，暂不考虑

综合分析后，我们选方案3

```cpp
//Rule.h
...
template<typename Head>
Head allof(Head head)
{
    return head;
}
template<typename Head, typename... Tail>
Head allof(Head head, Tail... tail)
{
    return head + allof<Head>(tail...);
}

template<typename Head>
Head anyof(Head head)
{
    if(!head.empty()) return head;
    return std::string("");
}

template<typename Head, typename... Tail>
Head anyof(Head head, Tail... tail)
{
    if(!head.empty()) return head;
    return anyof<Head>(tail...);
}
```
```cpp
//CountOffGame.cpp

#include "CountOffGame.h"
#include "Rule.h"
#include "Predicate.h"
#include "Action.h"

std::string CountOffGame::countOff(int n) const
{
    auto r1_1 = rule<times, shout>(n, 3, "石头");
    auto r1_2 = rule<times, shout>(n, 5, "剪刀");
    auto r1_3 = rule<times, shout>(n, 7, "布");
    auto r1 = allof(r1_1, r1_2, r1_3);

    auto r2 = rule<contains, shout>(n, 3, "石头");
    auto rd = rule<always_true, nop>(n, 0, "");

    return anyof(r2, r1, rd);
}
```
现在我们彻底解决了规则管理问题，你可以任意调整优先级了，也可以任意忽略规则了。在整个设计过程中，我们发现重复总是我们识别坏味道最好的引子。当变化方向来的时候，也就是我们被第二颗子弹击中的时候（好吧，有时候是第三颗子弹），我们需要把该方向的所有问题解决，而不是仅解决该问题。

>注： 坦白讲，这里的`CountOffGame`类使用的完全没有必要，你完全可以使用一个简单的 `const char* count_off(int n)`函数代替，再把`std::string`使用`char*`取代，这样就完全是一份c的代码了:-)

---

## Sprint 3

>需求 3:
>a. 第二天，幼儿园换了新老师，新老师对游戏进行了修改
    三个特殊的个位数变更为5,7,9。规则1，规则2依然有效，例如：
        遇到 5 的倍数说“石头”，7的倍数说“剪刀”，9的倍数说“布”；
        遇到 63 说“剪刀布”
        遇到 53 说“石头”，遇到35说“石头”
b. 需求1，2 测试用例断言不允许修改，仅允许修改前置条件

需求3已经说的很明显了，三个特殊数可以发生变化，并且需要通过游戏注入，而不需要修改游戏规则

简单重构测试用例断言，增加需求3用例

```cpp
#include "gtest/gtest.h"
#include "CountOffGame.h"

struct GameTest : testing::Test
{
    GameTest() : game_sprint_1_2(3, 5, 7)
               , game_sprint_3(5, 7, 9)
    {
    }

    void ASSERT_COUNTOFF_3_5_7(int n, const std::string& words)
    {
        ASSERT_EQ(words, game_sprint_1_2.countOff(n));
    }

    void ASSERT_COUNTOFF_5_7_9(int n, const std::string& words)
    {
        ASSERT_EQ(words, game_sprint_3.countOff(n));
    }

protected:
    CountOffGame game_sprint_1_2;
    CountOffGame game_sprint_3;
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
```

快速完成需求3，仅是规则中Predicate匹配内容发生了变化，修改CountOffGame即可
```cpp
//CountOffGame.h
#include <string>

struct CountOffGame
{
    CountOffGame(int n1, int n2, int n3);
    std::string countOff(int n) const;

private:
    int n1;
    int n2;
    int n3;
};

//CountOffGame.cpp
CountOffGame::CountOffGame(int n1, int n2, int n3) : n1(n1), n2(n2), n3(n3)
{
}

std::string CountOffGame::countOff(int n) const
{
    auto r1_1 = rule<times, shout>(n, n1, "石头");
    auto r1_2 = rule<times, shout>(n, n2, "剪刀");
    auto r1_3 = rule<times, shout>(n, n3, "布");
    auto r1 = allof(r1_1, r1_2, r1_3);

    auto r2 = rule<contains, shout>(n, n1, "石头");
    auto rd = rule<always_true, nop>(n, 0, "");

    return anyof(r2, r1, rd);
}
```

继续...

---
## Sprint 4

>需求 4:
>第三天，原来老师又回来了，对游戏又做了如下修改
a. 老师又把三个特殊的个位数变回为3,5,7
b. 规则1与规则2中“石头、剪刀、布”变更为 “老虎，棒子，鸡”。
c. 打印120位小朋友报数的结果到文件（count_off.txt），并提交到 document 文件夹下。可以不处理IO，打印出来拷贝到 count_off.txt

这压根没引入什么新的变化方向，仅是规则中Action的内容发生了变化，修改CountOffGame即可，我们爱死这个老师了！

增加需求4测试用例：

```cpp
struct GameTest : testing::Test
{
    GameTest() : game_sprint_1_2(3, 5, 7, "石头", "剪刀", "布")
               , game_sprint_3(5, 7, 9, "石头", "剪刀", "布")
               , game_sprint_4(3, 5, 7, "老虎", "棒子", "鸡")
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
```

快速实现需求

```cpp
//CountOffGame.h
#include <string>

struct CountOffGame
{
    CountOffGame(int n1, int n2, int n3, const std::string&, const std::string&, const std::string&);
    std::string countOff(int n) const;

private:
    int n1;
    int n2;
    int n3;
    const std::string w1;
    const std::string w2;
    const std::string w3;
};

//CountOffGame.cpp
CountOffGame::CountOffGame(int n1, int n2, int n3, const std::string& w1, const std::string& w2, const std::string& w3)
    : n1(n1), n2(n2), n3(n3), w1(w1), w2(w2), w3(w3)
{
}

std::string CountOffGame::countOff(int n) const
{
    auto r1_1 = rule<times, shout>(n, n1, w1);
    auto r1_2 = rule<times, shout>(n, n2, w2);
    auto r1_3 = rule<times, shout>(n, n3, w3);
    auto r1 = allof(r1_1, r1_2, r1_3);

    auto r2 = rule<contains, shout>(n, n1, w1);
    auto rd = rule<always_true, nop>(n, 0, "");

    return anyof(r2, r1, rd);
}
```

虽然实现了需求，但这一长串初始化参数，搞的人都快吐了，
仔细分析一下，报数游戏中规则条件及动作结果是一个一一映射关系，我们可以定义一个`RuleMap`进行化简

```cpp
//CountOffGame.h
#include <string>
#include <initializer_list>
#include <vector>

struct RuleMap
{
    int n;
    std::string words;
};

struct CountOffGame
{
    CountOffGame(const std::initializer_list<RuleMap>&);
    std::string countOff(int n) const;

private:
    std::vector<RuleMap> rules;
};

//CountOffGame.cpp
#include "CountOffGame.h"
#include "Rule.h"
#include "Predicate.h"
#include "Action.h"

CountOffGame::CountOffGame(const std::initializer_list<RuleMap>& rules) : rules(rules)
{
}

std::string CountOffGame::countOff(int n) const
{
    auto r1_1 = rule<times, shout>(n, rules[0].n, rules[0].words);
    auto r1_2 = rule<times, shout>(n, rules[1].n, rules[1].words);
    auto r1_3 = rule<times, shout>(n, rules[2].n, rules[2].words);
    auto r1 = allof(r1_1, r1_2, r1_3);

    auto r2 = rule<contains, shout>(n, rules[0].n, rules[0].words);
    auto rd = rule<always_true, nop>(n, 0, "");

    return anyof(r2, r1, rd);
}
```

每个规则中`rules[...]`又出现了重复，继续消除

```cpp
//rule.h
#include <string>

struct RuleMap
{
    int n;
    std::string words;
};

typedef bool (*Predicate)(int, int);
typedef std::string (*Action)(int, const std::string& );

template<Predicate isMatch, Action do>
std::string rule(int n, const RuleMap& map = {})
{
    if(isMatch(n, map.n))
    {
        return do(n, map.words);
    }

    return std::string("");
}
...
```

```cpp
//CountOffGame.cpp
...
std::string CountOffGame::countOff(int n) const
{
    auto r1_1 = rule<times, shout>(n, rules[0]);
    auto r1_2 = rule<times, shout>(n, rules[1]);
    auto r1_3 = rule<times, shout>(n, rules[2]);
    auto r1 = allof(r1_1, r1_2, r1_3);

    auto r2 = rule<contains, shout>(n, rules[0]);
    auto rd = rule<always_true, nop>(n);

    return anyof(r2, r1, rd);
}
```

yongshunli@163.com @ November 26, 2017 11:31 PM
