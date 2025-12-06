//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Core.h>
#include <Framework/Core/Utility/DI.h>

using namespace ob;

namespace test {

    class ServiceNull {
    public:
        ServiceNull() {

        }
    };


    // 参照なし
    class ServiceA {
    public:
        ServiceA() {

        }
    };

    // B -> A
    class ServiceB {
    public:
        ServiceB(ServiceA&) {

        }
    };

    // C --> B --> A
    class ServiceC {
    public:
        ServiceC(ServiceB&) {

        }
    };

	// D --> A
    // |     ↑
    // └---> B
    class ServiceD {
    public:
        ServiceD(ServiceA&, ServiceB&) {

        }
    };

    class ServiceE {
    public:
        ServiceE() {

        }
    };

    // F --> E
    class ServiceF {
    public:
        ServiceF(ServiceE&) {

        }
    };

    class ServiceG {
    public:
        ServiceG(ServiceA*) {

        }
    };

    // H --> A&
    // H --> E*
    class ServiceH {
    public:
        ServiceH(ServiceA&,ServiceE*) {

        }
    };

    // I --> const A&
    // I --> const E*
    class ServiceI {
    public:
        ServiceI(const ServiceA&, const ServiceE*) {

        }
    };

    // J --> null
    class ServiceJ {
    public:
        ServiceJ(const ServiceNull*) {

        }
    };

    // 継承
    class ServiceKBase {
    public:
        virtual ~ServiceKBase() = default;
    };
    class ServiceK1 : public ServiceKBase {
    public:
        ServiceK1() {}
    };
    class ServiceK2 : public ServiceKBase {
    public:
        ServiceK2() {}
    };
    class ServiceK3 : public ServiceKBase {
    public:
        ServiceK3() {
			throw Exception("生成禁止");
        }
    };


}

using namespace test;

void Inject(ServiceInjector& injector) {
    injector.bind<ServiceA>();
    injector.bind<ServiceB>();
    injector.bind<ServiceC>();
    injector.bind<ServiceD>();
    injector.bind<ServiceE>();
    injector.bind<ServiceF>();
    injector.bind<ServiceG>();
    injector.bind<ServiceH>();
    injector.bind<ServiceI>();
    injector.bind<ServiceJ>();
    injector.bind<ServiceK1>().as<ServiceKBase>();
    injector.bind<ServiceK2>().as<ServiceKBase>();
    injector.bind<ServiceK3>().as<ServiceKBase>();
}

TEST(DI, CreateMono)
{
    ServiceContainer container;
    ServiceInjector injector;
    Inject(injector);
    injector.create<ServiceA>(container);

    EXPECT_TRUE(container.get<ServiceA>());
}

TEST(DI, CreateDependency)
{
    ServiceContainer container;
    ServiceInjector injector;
    Inject(injector);
    injector.create<ServiceB>(container);
    
    EXPECT_TRUE(container.get<ServiceA>());
    EXPECT_TRUE(container.get<ServiceB>());
}

TEST(DI, CreateRecursiveDependency)
{
    ServiceContainer container;
    ServiceInjector injector;
    Inject(injector);
    injector.create<ServiceC>(container);

    EXPECT_TRUE(container.get<ServiceA>());
    EXPECT_TRUE(container.get<ServiceB>());
    EXPECT_TRUE(container.get<ServiceC>());
}

TEST(DI, CreateMultiDependency)
{
    ServiceContainer container;
    ServiceInjector injector;
    Inject(injector);
    injector.create<ServiceD>(container);

    EXPECT_TRUE(container.get<ServiceA>());
    EXPECT_TRUE(container.get<ServiceB>());
    EXPECT_TRUE(!container.get<ServiceC>());
    EXPECT_TRUE(container.get<ServiceD>());
}

TEST(DI, CreateMultiCreate)
{
    ServiceContainer container;
    ServiceInjector injector;
    Inject(injector);
    injector.createAll<ServiceA,ServiceF>(container);

    EXPECT_TRUE(container.get<ServiceA>());
    EXPECT_TRUE(container.get<ServiceE>());
    EXPECT_TRUE(container.get<ServiceF>());
}

TEST(DI, CreateOptional)
{
    ServiceContainer container;
    ServiceInjector injector;
    Inject(injector);
    injector.create<ServiceG>(container);
}

TEST(DI, CreateMixed)
{
    ServiceContainer container;
    ServiceInjector injector;
    Inject(injector);
    injector.create<ServiceH>(container);

    EXPECT_TRUE(container.get<ServiceA>());
    EXPECT_TRUE(container.get<ServiceH>());
}

TEST(DI, CreateConst)
{
    ServiceContainer container;
    ServiceInjector injector;
    Inject(injector);
    injector.create<ServiceI>(container);

    EXPECT_TRUE(container.get<ServiceA>());
    EXPECT_TRUE(container.get<ServiceE>());
    EXPECT_TRUE(container.get<ServiceI>());
}

TEST(DI, CreateNull)
{
    ServiceContainer container;
    ServiceInjector injector;
    Inject(injector);
    injector.create<ServiceJ>(container);

    EXPECT_TRUE(container.get<ServiceJ>());
    EXPECT_TRUE(!container.get<ServiceNull>());
}

TEST(DI, CreateInherit)
{
    ServiceContainer container;
    ServiceInjector injector;
    Inject(injector);
    injector.create<ServiceKBase>(container);

    EXPECT_TRUE(container.get<ServiceKBase>());
    EXPECT_TRUE(container.get<ServiceK1>());
    EXPECT_TRUE(!container.get<ServiceK2>());
    EXPECT_TRUE(!container.get<ServiceK3>());
}

TEST(DI, CreateInheritDirect)
{
    ServiceContainer container;
    ServiceInjector injector;
    Inject(injector);
    injector.create<ServiceK2>(container);

    EXPECT_TRUE(container.get<ServiceKBase>());
    EXPECT_TRUE(container.get<ServiceK2>());
}

TEST(DI, CreateCancel)
{
    ServiceContainer container;
    ServiceInjector injector;
    Inject(injector);
    injector.create<ServiceK3>(container);

    EXPECT_TRUE(!container.get<ServiceKBase>());
    EXPECT_TRUE(!container.get<ServiceK1>());
    EXPECT_TRUE(!container.get<ServiceK2>());
    EXPECT_TRUE(!container.get<ServiceK3>());
    EXPECT_TRUE(!container.get<ServiceK3>());
}

TEST(DI, CreateAll)
{
    ServiceContainer container;
    ServiceInjector injector;
    Inject(injector);
    injector.createAll(container);

    EXPECT_TRUE(container.get<ServiceA>());
    EXPECT_TRUE(container.get<ServiceB>());
    EXPECT_TRUE(container.get<ServiceC>());
    EXPECT_TRUE(container.get<ServiceD>());
    EXPECT_TRUE(container.get<ServiceE>());
    EXPECT_TRUE(container.get<ServiceF>());
    EXPECT_TRUE(container.get<ServiceG>());
    EXPECT_TRUE(container.get<ServiceH>());
    EXPECT_TRUE(container.get<ServiceI>());
}