#include "crow.h"
#include "storage/PostgresRepository.hpp"
#include "ui/common/WrapCore.hpp"
#include <memory>

int main()
{
    auto repo = std::make_shared<PostgresRepository>();
    auto currentSet = std::make_shared<IntegerSet>();
    WrapCore core(repo, currentSet);
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")(
        [](){ return "Integer Set Lab API is Online!";}
    );
    
    CROW_ROUTE(app, "/sets")(
        [&core]()
        {
            auto ids = core.getIdList();
            nlohmann::json j = ids;
            return crow::response(j.dump());
        }
    );

    app.port(18080).multithreaded().run();
}
