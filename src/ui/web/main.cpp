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
        [](const crow::request&, crow::response& res)
        {
            res.set_static_file_info("index.html");
            res.end();
        }
    );

    CROW_ROUTE(app, "/sets")(
        [&core]()
        {
            auto ids = core.getIdList();
            nlohmann::json j = ids;
            return crow::response(j.dump());
        }
    );

    CROW_ROUTE(app, "/sets/<uint>")(
        [&core](size_t id)
        {
            std::string json = core.getSetJson(id);
            if (json == "{}") return crow::response(404, "Not Found");
            return crow::response(json);
        }
    );

    CROW_ROUTE(app, "/api/sets/<uint>").methods(crow::HTTPMethod::DELETE)(
        [core](size_t id)
        {
            if (core->removeSet(id)) return crow::response(204);
            return crow::response(404, "ID not found");
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

    CROW_ROUTE(app, "/math/<string>")
    (
        [&core](std::string opType) { return crow::response(200, "Logic for " + opType + " goes here");}
    );

    app.port(18080).multithreaded().run();
}
