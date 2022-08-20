//***********************************************************
//! @file
//! @brief		handle_forward_listテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Template/Container/HandleForwardList.h>
using namespace ob;

struct Hoge {
    Hoge(int a, int b) :val(a* b) {}
    Hoge(const Hoge&) = default;
    Hoge& operator=(const Hoge&) = default;
    Hoge(Hoge&&) = default;
    Hoge& operator=(Hoge&&) = default;
    int val = 0;
};

using HList = HandleForwardList<Hoge>;
using Handle = HList::Handle;
TEST(HandleForwardList, Construct) {

    // ムーブ・コンストラクタ
    {
        HList lst;
        Handle h1, h2;
        lst.emplace_front(h1, 2, 3);
        lst.emplace_front(h2, 5, 7);

        HList lst2 = std::move(lst);

        int sum2 = 0;
        std::for_each(lst2.begin(), lst2.end(), [&sum2](auto& x) {sum2 += x.val; });
        EXPECT_EQ(sum2, 2 * 3 + 5 * 7);
    }

    // ムーブ代入
    {
        HList lst;
        Handle h1, h2;
        lst.emplace_front(h1, 2, 3);
        lst.emplace_front(h2, 5, 7);

        HList lst2;
        lst2 = std::move(lst);

        int sum2 = 0;
        std::for_each(lst2.begin(), lst2.end(), [&sum2](auto& x) {sum2 += x.val; });
        EXPECT_EQ(sum2, 2 * 3 + 5 * 7);
    }

}

TEST(HandleForwardList, Iterator) {

    // begin() / end()
    {
        HList lst;
        Handle h1, h2, h3;
        lst.emplace_front(h3, 1, 7);
        lst.emplace_front(h2, 1, 5);
        lst.emplace_front(h1, 1, 3);

        int val = 0;
        std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });

        EXPECT_EQ(val, 357);
    }

    // cbegin() / cend()
    {
        HList lst;
        Handle h1, h2, h3;
        lst.emplace_front(h3, 1, 7);
        lst.emplace_front(h2, 1, 5);
        lst.emplace_front(h1, 1, 3);

        int val = 0;
        std::for_each(lst.cbegin(), lst.cend(), [&val](auto& x) {val *= 10; val += x.val; });

        EXPECT_EQ(val, 357);
    }

}

TEST(HandleForwardList, Func) {

    // empty() / size()
    {
        HList lst;
        EXPECT_TRUE(lst.empty());
        EXPECT_EQ(lst.size(), 0);

        Handle h1, h2, h3;
        lst.emplace_front(h1, 1, 3);
        lst.emplace_front(h2, 1, 5);
        lst.emplace_front(h3, 1, 7);

        EXPECT_FALSE(lst.empty());
        EXPECT_EQ(lst.size(), 3);
    }

    // front() / back()
    {
        HList lst;
        Handle h1, h2, h3;
        lst.emplace_front(h1, 2, 3);
        lst.emplace_front(h2, 2, 5);
        lst.emplace_front(h3, 2, 7);

        EXPECT_EQ(lst.front().val, 2 * 7);
    }

    // swap()
    {
        HList lst;
        Handle h1, h2, h3;
        lst.emplace_front(h3, 1, 3);
        lst.emplace_front(h2, 1, 2);
        lst.emplace_front(h1, 1, 1);

        HList lst2;
        Handle h4;
        lst2.emplace_front(h4, 1, 4);

        lst.swap(lst2);

        int val = 0;
        std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 4);

        val = 0;
        std::for_each(lst2.begin(), lst2.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 123);
    }

    // reverse()
    {
        HList lst;
        Handle h1, h2, h3, h4;
        lst.emplace_front(h4, 1, 4);
        lst.emplace_front(h3, 1, 3);
        lst.emplace_front(h2, 1, 2);
        lst.emplace_front(h1, 1, 1);

        lst.reverse();

        int val = 0;
        std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 4321);
    }

}

TEST(HandleForwardList, AddRemove) {

    // push_front() / emplace_front()
    {
        HList lst;
        Handle h1, h2, h3, h4;
        lst.emplace_front(h1, 1, 3);
        lst.push_front(h2, Hoge(1, 5));
        Hoge hoge(1, 7);
        lst.push_front(h3, hoge);
        lst.emplace_front(h4, 1, 9);

        int val = 0;
        std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });

        EXPECT_EQ(val, 9753);
    }

    // insert() / emplace()
    {
        HList lst;
        Handle h1, h2, h3, h4, h5, h6;
        lst.emplace_front(h3, 1, 3);
        lst.emplace_front(h2, 1, 2);
        lst.emplace_front(h1, 1, 1);

        lst.insert_after(h4, lst.begin_before(), Hoge(1, 4));
        Hoge hoge(1, 5);
        lst.insert_after(h5, (lst.begin()), hoge);
        lst.emplace_after(h6, ++(++(++lst.begin())), 1, 6);

        int val = 0;
        std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });

        EXPECT_EQ(val, 451263);
    }

    // pop_front() / pop_back()
    {
        HList lst;
        Handle h1, h2, h3, h4;
        lst.emplace_front(h4, 1, 4);
        lst.emplace_front(h3, 1, 3);
        lst.emplace_front(h2, 1, 2);
        lst.emplace_front(h1, 1, 1);

        lst.pop_front();
        lst.pop_front();

        int val = 0;
        std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 34);
    }

}

TEST(HandleForwardList, Erase) {

    // erase_after()
    {
        HList lst;
        Handle h1, h2, h3, h4;
        lst.emplace_front(h4, 1, 4);
        lst.emplace_front(h3, 1, 3);
        lst.emplace_front(h2, 1, 2);
        lst.emplace_front(h1, 1, 1);

        lst.erase_after(++lst.begin());

        int val = 0;
        std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 124);
    }

    // erase()
    {
        HList lst;
        Handle h1, h2, h3, h4;
        lst.emplace_front(h4, 1, 4);
        lst.emplace_front(h3, 1, 3);
        lst.emplace_front(h2, 1, 2);
        lst.emplace_front(h1, 1, 1);

        lst.erase_after(++lst.begin(), ++(++lst.begin()));

        int val = 0;
        std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 124);
    }

    // clear()
    {
        HList lst;
        Handle h1, h2, h3, h4;
        lst.emplace_front(h4, 1, 4);
        lst.emplace_front(h3, 1, 3);
        lst.emplace_front(h2, 1, 2);
        lst.emplace_front(h1, 1, 1);

        lst.clear();
        EXPECT_TRUE(lst.empty());
        EXPECT_EQ(lst.size(), 0);

        lst.emplace_front(h2, 1, 3);
        lst.emplace_front(h1, 1, 1);

        int val = 0;
        std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 13);
    }

    // remove()
    {
        HList lst;
        Handle h1, h2, h3, h4;
        lst.emplace_front(h4, 1, 4);
        lst.emplace_front(h3, 1, 3);
        lst.emplace_front(h2, 1, 2);
        lst.emplace_front(h1, 1, 1);

        lst.remove(h1);

        int val = 0;
        std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 234);

        h3.remove();

        val = 0;
        std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 24);
    }

    // remove_if()
    {
        HList lst;
        Handle h1, h2, h3, h4;
        lst.emplace_front(h4, 1, 4);
        lst.emplace_front(h3, 1, 3);
        lst.emplace_front(h2, 1, 2);
        lst.emplace_front(h1, 1, 1);

        lst.remove_if([](const Hoge& pred) {return pred.val % 2; });

        int val = 0;
        std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 24);
    }

}

TEST(HandleForwardList, Scope) {

    // scope
    {
        HList lst;

        Handle h1;
        lst.emplace_front(h1, 1, 1);
        {
            Handle h2;
            lst.emplace_front(h2, 1, 2);
            {
                Handle h3;
                lst.emplace_front(h3, 1, 3);
            }
            {
                int val = 0;
                std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
                EXPECT_EQ(val, 21);
            }
            {
                Handle h3, h4;
                lst.emplace_front(h3, 1, 3);
                lst.emplace_front(h4, 1, 4);
            }
            {
                int val = 0;
                std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
                EXPECT_EQ(val, 21);
            }
        }

        {
            int val = 0;
            std::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
            EXPECT_EQ(val, 1);
        }

    }
}