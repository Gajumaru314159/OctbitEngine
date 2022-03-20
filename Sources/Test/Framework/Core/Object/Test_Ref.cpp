/*
//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Foundation/Memory/Reference/Ref.h>
#include <Framework/Foundation/Memory/Reference/RefObject.h>
using namespace ob;

namespace {

    class InvalidBase {

    };

    class Base :public RefObject {
    public:
        Base() = default;
        Base(s32 val) :m_val(val) {}
        s32 m_val = 100;
    };

    class Derived :public Base {
    public:
        Derived() = default;
        Derived(s32 val) :Base(0) {}
    };

    int TestFunc(const Ref<Base>& ref) {
        if (!ref)return 0;
        return ref->m_val;
    }
}


TEST(RefObject, Construct) {

    // 空初期化
    {
        Ref<Base> ref1;
        Ref<Base> ref2(nullptr);

        EXPECT_EQ(nullptr, ref1.get());
        EXPECT_EQ(nullptr, ref2.get());
    }
    // 生ポインタ初期化
    {
        Base obj1;
        Derived obj2;
        EXPECT_EQ(0, RefObjectHelper::getReferenceCount(&obj1));
        EXPECT_EQ(0, RefObjectHelper::getReferenceCount(&obj2));
        {
            // Base => get<Base>
            Ref<Base> ref1(&obj1);
            EXPECT_EQ(1, RefObjectHelper::getReferenceCount(&obj1));
            EXPECT_EQ(&obj1, ref1.get());

            // Derived => get<Base>
            Ref<Base> ref2(&obj2);
            EXPECT_EQ(1, RefObjectHelper::getReferenceCount(&obj2));
            EXPECT_EQ(&obj2, ref2.get());
        }
        EXPECT_EQ(0, RefObjectHelper::getReferenceCount(&obj1));
        EXPECT_EQ(0, RefObjectHelper::getReferenceCount(&obj2));
    }

    // コピー
    {
        Base obj1;
        Derived obj2;
        {
            // Base => get<Base>
            Ref<Base> ref1(&obj1);
            Ref<Base> ref1_2(ref1);
            EXPECT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));
            EXPECT_EQ(&obj1, ref1_2.get());

            // get<Derived> => get<Base>
            Ref<Derived> ref2(&obj2);
            Ref<Base> ref2_2(ref2);
            EXPECT_EQ(2, RefObjectHelper::getReferenceCount(&obj2));
            EXPECT_EQ(&obj2, ref2_2.get());
        }
        EXPECT_EQ(0, RefObjectHelper::getReferenceCount(&obj1));
        EXPECT_EQ(0, RefObjectHelper::getReferenceCount(&obj2));
    }

    // ムーブ
    {
        Base obj1;
        Derived obj2;
        {
            // Base => get<Base>
            Ref<Base> ref1(&obj1);
            Ref<Base> ref1_2(move(ref1));
            EXPECT_EQ(1, RefObjectHelper::getReferenceCount(&obj1));
            EXPECT_EQ(&obj1, ref1_2.get());

            // get<Derived> => get<Base>
            Ref<Derived> ref2(&obj2);
            Ref<Base> ref2_2(move(ref2));
            EXPECT_EQ(1, RefObjectHelper::getReferenceCount(&obj2));
            EXPECT_EQ(&obj2, ref2_2.get());
        }
        EXPECT_EQ(0, RefObjectHelper::getReferenceCount(&obj1));
        EXPECT_EQ(0, RefObjectHelper::getReferenceCount(&obj2));
    }


    {
        // IRefObjectを継承していない場合コンパイルエラー
        //get<InvalidBase> invalidRef;
    }

    // 暗黙的キャスト
    {
        Base obj(3);
        Ref<Base> ref(&obj);
        Base* ptr = ref;
        EXPECT_EQ(3, ptr->m_val);
    }
}


TEST(RefObject, Assign) {
    Base obj1;
    Derived obj2;
    Ref<Base> ref1(&obj1);
    Ref<Derived> ref2(&obj2);
    Ref<Derived> nullref;

    //* [ ] copy
    {
        Ref<Base> ref3;
        Ref<Base> ref4;

        // null => OBJ
        ref3 = ref1;
        ref4 = ref2;
        EXPECT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));
        EXPECT_EQ(2, RefObjectHelper::getReferenceCount(&obj2));
        EXPECT_EQ(&obj1, ref3.get());
        EXPECT_EQ(&obj2, ref4.get());

        // OBJ => null
        ref3 = nullptr;
        ref4 = nullptr;
        EXPECT_EQ(1, RefObjectHelper::getReferenceCount(&obj1));
        EXPECT_EQ(1, RefObjectHelper::getReferenceCount(&obj2));
        EXPECT_EQ(nullptr, ref3.get());
        EXPECT_EQ(nullptr, ref4.get());

        // OBJ1 => OBJ2
        ref3 = ref1;
        ref4 = ref2;
        ref3 = nullref;
        ref4 = nullref;
        EXPECT_EQ(1, RefObjectHelper::getReferenceCount(&obj1));
        EXPECT_EQ(1, RefObjectHelper::getReferenceCount(&obj2));
        EXPECT_EQ(nullptr, ref3.get());
        EXPECT_EQ(nullptr, ref4.get());

        // self assign
        ref3 = ref1;
        EXPECT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));
        ref3 = ref3;
        EXPECT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));
        EXPECT_EQ(&obj1, ref3.get());
    }

    //* [ ] move
    {
        Ref<Base> ref3;
        Ref<Base> ref4;

        ref3 = std::move(ref1);
        ref4 = std::move(ref2);
        EXPECT_EQ(1, RefObjectHelper::getReferenceCount(&obj1));
        EXPECT_EQ(1, RefObjectHelper::getReferenceCount(&obj2));
        EXPECT_EQ(nullptr, ref1.get());
        EXPECT_EQ(nullptr, ref2.get());
        EXPECT_EQ(&obj1, ref3.get());
        EXPECT_EQ(&obj2, ref4.get());

        // self assign
        ref1 = &obj1;
        ref3 = ref1;
        EXPECT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));
        ref3 = std::move(ref3);
        EXPECT_EQ(2, RefObjectHelper::getReferenceCount(&obj1));
        EXPECT_EQ(&obj1, ref3.get());
    }

    ref1 = nullptr;
    ref2 = nullptr;
    EXPECT_EQ(0, RefObjectHelper::getReferenceCount(&obj1));
    EXPECT_EQ(0, RefObjectHelper::getReferenceCount(&obj2));

    //* [ ] raw ptr
    {
        Ref<Base> ref3;
        Ref<Base> ref4;

        ref3 = &obj1;
        ref4 = &obj2;
        EXPECT_EQ(1, RefObjectHelper::getReferenceCount(&obj1));
        EXPECT_EQ(1, RefObjectHelper::getReferenceCount(&obj2));
    }
    EXPECT_EQ(0, RefObjectHelper::getReferenceCount(&obj1));
    EXPECT_EQ(0, RefObjectHelper::getReferenceCount(&obj2));
}

TEST(RefObject, reset) {
    Base obj1;

    Ref<Base> ref1;
    ref1.reset();
    EXPECT_EQ(nullptr, ref1.get());

    ref1.reset(&obj1);
    EXPECT_EQ(1, RefObjectHelper::getReferenceCount(&obj1));
    EXPECT_EQ(&obj1, ref1.get());

    ref1.reset();
    EXPECT_EQ(0, RefObjectHelper::getReferenceCount(&obj1));
    EXPECT_EQ(nullptr, ref1.get());
}

TEST(RefObject, Operator) {
    Base obj1(100);
    Ref<Base> ref1(&obj1);
    Ref<Base> ref2;


    EXPECT_EQ(100, (*ref1).m_val);
    EXPECT_EQ(100, ref1->m_val);

    bool r1 = ref1;
    EXPECT_EQ(true, r1);

    bool r2 = ref2;
    EXPECT_EQ(false, r2);


    ref1 = &obj1;
    ref2 = &obj1;
    EXPECT_EQ(true, ref1 == ref2);

    ref1 = &obj1;
    ref2 = nullptr;
    EXPECT_EQ(false, ref1 == ref2);

    // 比較演算子
    bool ab = ref1 < ref2;
    bool ba = ref1 > ref2;
    bool eq = ref1 == ref2;

    EXPECT_EQ(ref1 <= ref2, ab || eq);
    EXPECT_EQ(ref1 >= ref2, ba || eq);
}
*/