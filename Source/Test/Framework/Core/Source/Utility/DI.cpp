//***********************************************************
//! @file
//! @brief		Path のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>

using namespace ob;

namespace test {

    static int s_log = 0;

    // ログ
    class Logger {
    public:
        static void Register(DependencyGraph& graph) {
            graph.add<Logger>();
        }
    public:
        Logger(ServiceContainer& container) {

        }
        void print(int value) {
            s_log = value;
        }
    };

    // オペレータ・インターフェイス
    class Operator {
    public:
        virtual int eval(int a, int b) = 0;
    };
    // +
    class Adder :public Operator {
    public:
        static void Register(DependencyGraph& graph) {
            graph.add<Adder>()
                .as<Operator>();
        }
    public:
        Adder(ServiceContainer& container) {

        }
        int eval(int a, int b) override {
            return a + b;
        }
    };
    // *
    class Multiplier :public Operator {
    public:
        static void Register(DependencyGraph& graph) {
            graph.add<Multiplier>()
                .as<Operator>();
        }
    public:
        Multiplier(ServiceContainer& container) {

        }
        int eval(int a, int b) override {
            return a * b;
        }
    };
    // %
    class Modder :public Operator {
    public:
        static void Register(DependencyGraph& graph) {
            graph.add<Adder>()
                .as<Operator>();
        }
    public:
        Modder() {

        }
        int eval(int a, int b) override {
            return a % b;
        }
    };
    // 計算機
    class Calculator {
    public:
        static void Register(DependencyGraph& graph) {
            graph.add<Calculator>()
                .require<Operator>()
                .optional<Logger>();
        }
    public:
        Calculator(ServiceContainer& container) 
            : m_container(container)
        {

        }

        void calc(int a, int b) {
            auto result = m_container.ref<Operator>().eval(a,b);

            if (auto logger = m_container.get<Logger>()) {
                logger->print(result);
            }
        }
    private:
        ServiceContainer& m_container;
    };
}

TEST(DI, Constructors)
{
    {
        DependencyGraph graph;
        test::Calculator::Register(graph);
        test::Adder::Register(graph);
        test::Logger::Register(graph);

        ServiceContainer container{ graph };

        container.ref<test::Calculator>().calc(2, 3);
        EXPECT_EQ(test::s_log, 5);
    }
    {
        DependencyGraph graph;
        test::Calculator::Register(graph);
        test::Multiplier::Register(graph);
        test::Logger::Register(graph);

        ServiceContainer container{ graph };

        container.ref<test::Calculator>().calc(2, 3);
        EXPECT_EQ(test::s_log, 6);
    }
    {
        DependencyGraph graph;
        test::Modder modder;
        graph.add(modder).as<test::Operator>();
        test::Calculator::Register(graph);
        test::Logger::Register(graph);

        ServiceContainer container{ graph };

        container.ref<test::Calculator>().calc(10, 4);
        EXPECT_EQ(test::s_log, 2);
    }
}