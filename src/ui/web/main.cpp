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

    CROW_ROUTE(app, "/create").methods(crow::HTTPMethod::POST)
    (
        [&core](const crow::request& req)
        {
            std::istringstream iss(req.body);
            if (core.createSet(iss))
            {
                size_t newId = core.saveSet();
                return crow::response(201, "Set created with ID: " + std::to_string(newId));
            }
            return crow::response(400, "Invalid set data");
        }
    );

    app.port(18080).multithreaded().run();
}
