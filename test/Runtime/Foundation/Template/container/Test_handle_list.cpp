//***********************************************************
//! @file
//! @brief		handle_listテスト
//! @author		Gajumaru
//***********************************************************
#include <Foundation/Template/container/handle_list.h>
using namespace ob;

struct Hoge {
    Hoge(int a, int b) :val(a* b) {}
    Hoge(const Hoge&) = default;
    Hoge& operator=(const Hoge&) = default;
    Hoge(Hoge&&) = default;
    Hoge& operator=(Hoge&&) = default;
    int val = 0;
};

TEST(HandleList, Test) {
    using List = handle_list<Hoge>;
    using Handle = List::handle;

    // ムーブ・コンストラクタ
    {
        List lst;
        Handle h1, h2;
        lst.emplace_back(h1, 2, 3);
        lst.emplace_back(h2, 5, 7);

        List lst2 = ob::move(lst);

        int sum2 = 0;
        ob::for_each(lst2.begin(), lst2.end(), [&sum2](auto& x) {sum2 += x.val; });
        EXPECT_EQ(sum2, 2 * 3 + 5 * 7);
    }

    // ムーブ代入
    {
        List lst;
        Handle h1, h2;
        lst.emplace_back(h1, 2, 3);
        lst.emplace_back(h2, 5, 7);

        List lst2;
        lst2 = ob::move(lst);

        int sum2 = 0;
        ob::for_each(lst2.begin(), lst2.end(), [&sum2](auto& x) {sum2 += x.val; });
        EXPECT_EQ(sum2, 2 * 3 + 5 * 7);
    }

    // begin() / end()
    {
        List lst;
        Handle h1, h2, h3;
        lst.emplace_back(h1, 1, 3);
        lst.emplace_back(h2, 1, 5);
        lst.emplace_back(h3, 1, 7);

        int val = 0;
        ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });

        EXPECT_EQ(val, 357);
    }

    // cbegin() / cend()
    {
        List lst;
        Handle h1, h2, h3;
        lst.emplace_back(h1, 1, 3);
        lst.emplace_back(h2, 1, 5);
        lst.emplace_back(h3, 1, 7);

        int val = 0;
        ob::for_each(lst.cbegin(), lst.cend(), [&val](auto& x) {val *= 10; val += x.val; });

        EXPECT_EQ(val, 357);
    }

    // rbegin() / rend()
    {
        List lst;
        Handle h1, h2, h3;
        lst.emplace_back(h1, 1, 3);
        lst.emplace_back(h2, 1, 5);
        lst.emplace_back(h3, 1, 7);

        int val = 0;
        ob::for_each(lst.rbegin(), lst.rend(), [&val](auto& x) {val *= 10; val += x.val; });

        EXPECT_EQ(val, 753);
    }

    // crbegin() / crend()
    {
        List lst;
        Handle h1, h2, h3;
        lst.emplace_back(h1, 1, 3);
        lst.emplace_back(h2, 1, 5);
        lst.emplace_back(h3, 1, 7);

        int val = 0;
        ob::for_each(lst.crbegin(), lst.crend(), [&val](const auto& x) {val *= 10; val += x.val; });

        EXPECT_EQ(val, 753);
    }

    // empty() / size()
    {
        List lst;
        EXPECT_TRUE(lst.empty());
        EXPECT_EQ(lst.size(), 0);

        Handle h1, h2, h3;
        lst.emplace_back(h1, 1, 3);
        lst.emplace_back(h2, 1, 5);
        lst.emplace_back(h3, 1, 7);

        EXPECT_FALSE(lst.empty());
        EXPECT_EQ(lst.size(), 3);
    }

    // front() / back()
    {
        List lst;
        Handle h1, h2, h3;
        lst.emplace_back(h1, 2, 3);
        lst.emplace_back(h2, 2, 5);
        lst.emplace_back(h3, 2, 7);

        EXPECT_EQ(lst.front().val, 2 * 3);
        EXPECT_EQ(lst.back().val, 2 * 7);
    }

    // push_front() / emplace_front()
    {
        List lst;
        Handle h1, h2, h3, h4;
        lst.emplace_back(h1, 1, 3);
        lst.push_front(h2, Hoge(1, 5));
        Hoge hoge(1, 7);
        lst.push_front(h3, hoge);
        lst.emplace_front(h4, 1, 9);

        int val = 0;
        ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });

        EXPECT_EQ(val, 9753);
    }

    // push_back() / emplace_back()
    {
        List lst;
        Handle h1, h2, h3, h4;
        lst.emplace_back(h1, 1, 3);
        lst.push_back(h2, Hoge(1, 5));
        Hoge hoge(1, 7);
        lst.push_back(h3, hoge);
        lst.emplace_back(h4, 1, 9);

        int val = 0;
        ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });

        EXPECT_EQ(val, 3579);
    }

    // insert() / emplace()
    {
        List lst;
        Handle h1, h2, h3, h4, h5, h6;
        lst.emplace_back(h1, 1, 1);
        lst.emplace_back(h2, 1, 2);
        lst.emplace_back(h3, 1, 3);

        lst.insert(h4, lst.begin(), Hoge(1, 4));
        Hoge hoge(1, 5);
        lst.insert(h5, (++lst.begin()), hoge);
        lst.emplace(h6, --(--lst.end()), 1, 6);

        int val = 0;
        ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10;val += x.val; });

        EXPECT_EQ(val, 451623);
    }

    // pop_front() / pop_back()
    {
        List lst;
        Handle h1, h2, h3, h4;
        lst.emplace_back(h1, 1, 1);
        lst.emplace_back(h2, 1, 2);
        lst.emplace_back(h3, 1, 3);
        lst.emplace_back(h4, 1, 4);

        lst.pop_back();
        lst.pop_front();
        lst.pop_front();

        int val = 0;
        ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 3);
    }

    // erase()
    {
        List lst;
        Handle h1, h2, h3, h4;
        lst.emplace_back(h1, 1, 1);
        lst.emplace_back(h2, 1, 2);
        lst.emplace_back(h3, 1, 3);
        lst.emplace_back(h4, 1, 4);

        lst.erase(++lst.begin());

        int val = 0;
        ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 134);
    }

    // erase()
    {
        List lst;
        Handle h1, h2, h3, h4;
        lst.emplace_back(h1, 1, 1);
        lst.emplace_back(h2, 1, 2);
        lst.emplace_back(h3, 1, 3);
        lst.emplace_back(h4, 1, 4);

        lst.erase(++lst.begin(), ++(++lst.begin()));

        int val = 0;
        ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 134);
    }

    // clear()
    {
        List lst;
        Handle h1, h2, h3, h4;
        lst.emplace_back(h1, 1, 1);
        lst.emplace_back(h2, 1, 2);
        lst.emplace_back(h3, 1, 3);
        lst.emplace_back(h4, 1, 4);

        lst.clear();
        EXPECT_TRUE(lst.empty());
        EXPECT_EQ(lst.size(), 0);

        lst.emplace_back(h1, 1, 1);
        lst.emplace_back(h2, 1, 3);

        int val = 0;
        ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 13);
    }

    // swap()
    {
        List lst;
        Handle h1, h2, h3;
        lst.emplace_back(h1, 1, 1);
        lst.emplace_back(h2, 1, 2);
        lst.emplace_back(h3, 1, 3);

        List lst2;
        Handle h4;
        lst2.emplace_back(h4, 1, 4);

        lst.swap(lst2);

        int val = 0;
        ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 4);

        val = 0;
        ob::for_each(lst2.begin(), lst2.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 123);
    }

    // remove()
    {
        List lst;
        Handle h1, h2, h3, h4;
        lst.emplace_back(h1, 1, 1);
        lst.emplace_back(h2, 1, 2);
        lst.emplace_back(h3, 1, 3);
        lst.emplace_back(h4, 1, 4);

        lst.remove(h1);

        int val = 0;
        ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 234);

        h3.remove();

        val = 0;
        ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 24);
    }

    // remove_if()
    {
        List lst;
        Handle h1, h2, h3, h4;
        lst.emplace_back(h1, 1, 1);
        lst.emplace_back(h2, 1, 2);
        lst.emplace_back(h3, 1, 3);
        lst.emplace_back(h4, 1, 4);

        lst.remove_if([](const Hoge& pred) {return pred.val % 2==0; });

        int val = 0;
        ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 13);
    }

    // reverse()
    {
        List lst;
        Handle h1, h2, h3, h4;
        lst.emplace_back(h1, 1, 1);
        lst.emplace_back(h2, 1, 2);
        lst.emplace_back(h3, 1, 3);
        lst.emplace_back(h4, 1, 4);

        lst.reverse();

        int val = 0;
        ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
        EXPECT_EQ(val, 4321);
    }

    // scope
    {
        List lst;

        Handle h1;
        lst.emplace_back(h1, 1, 1);
        {
            Handle h2;
            lst.emplace_back(h2, 1, 2);
            {
                Handle h3;
                lst.emplace_back(h3, 1, 3);
            }
            {
                int val = 0;
                ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
                EXPECT_EQ(val, 12);
            }
            {
                Handle h3,h4;
                lst.emplace_back(h3, 1, 3);
                lst.emplace_back(h4, 1, 4);
            }
            {
                int val = 0;
                ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
                EXPECT_EQ(val, 12);
            }
        }

        {
            int val = 0;
            ob::for_each(lst.begin(), lst.end(), [&val](auto& x) {val *= 10; val += x.val; });
            EXPECT_EQ(val, 1);
        }

    }
}