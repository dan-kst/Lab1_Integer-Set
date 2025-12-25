#include "./storage/PostgresRepository.hpp"
#include "./storage/IntegerSetSerializer.hpp" 

PostgresRepository::~PostgresRepository(){}

void PostgresRepository::save(const IntegerSet& set, int id) {
    // 1. Connect to the DB (Use the credentials from your docker-compose)
    pqxx::connection c{"postgresql://user:password@localhost:5432/lab1_db"};
    pqxx::work txn{c};

    // 2. Prepare data (convert to string)
    std::string jsonData = SetSerializer::to_json(set).dump();

    // 3. Execute SQL
    c.prepare("insert_set", "INSERT INTO sets (id, data) VALUES ($1, $2) "
                           "ON CONFLICT (id) DO UPDATE SET data = $2");
    txn.exec_prepared("insert_set", id, jsonData);
    txn.commit();
}

std::unique_ptr<IntegerSet> PostgresRepository::load(int id)
{
    // 1. Connect to the DB (Use the credentials from your docker-compose)
    pqxx::connection c{"postgresql://user:password@localhost:5432/lab1_db"};
    pqxx::work txn{c};

    // 2. Execute SQL
    c.prepare("select_set", "SELECT * FROM sets WHERE id=$1");
    pqxx::row set =  txn.exec_prepared1("select_set", id);
    txn.commit();

    // 3. Parse data (convert field from string to vector<int>)
    std::string rawJson = set[1].as<std::string>();
    auto jsonDataObj = nlohmann::json::parse(rawJson);

    return SetSerializer::from_json(jsonDataObj);
}
