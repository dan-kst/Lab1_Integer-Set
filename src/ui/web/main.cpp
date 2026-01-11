#include "crow.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")(
        [](){ return "Integer Set Lab API is Online!";}
    );

    app.port(18080).multithreaded().run();
}
