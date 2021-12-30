#include <Runtime/Graphic/Public/System.h>
using namespace ob;

class App {
public:
    App(graphic::GraphicAPI api):
        m_logger(),
        m_graphicSystem(api){


    }

private:
    ob::Logger      m_logger;
    graphic::System m_graphicSystem;
};

int main() {

    App app(graphic::GraphicAPI::DirectX);

    
    return 0;
}