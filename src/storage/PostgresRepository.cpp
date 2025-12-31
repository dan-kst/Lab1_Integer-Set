#include "./storage/PostgresRepository.hpp"
#include "./storage/IntegerSetSerializer.hpp" 

PostgresRepository::~PostgresRepository(){}

size_t PostgresRepository::save(const IntegerSet& set) {
    // 1. Connect to the DB (Use the credentials from your docker-compose)
    pqxx::connection c{"postgresql://user:password@localhost:5432/lab1_db"};
    pqxx::work txn{c};

    // 2. Prepare data (convert to string)
    std::string jsonData = SetSerializer::to_json(set).dump();

    // 3. Execute SQL
    pqxx::row r = txn.exec_params1("INSERT INTO sets (data) VALUES ($1) RETURNING id", jsonData);
    // 4. Close connection and return results
    txn.commit();
    return r[0].as<int>();
}
std::unique_ptr<IntegerSet> PostgresRepository::load(size_t id)
{
    // 1. Connect to the DB (Use the credentials from your docker-compose)
    pqxx::connection c{"postgresql://user:password@localhost:5432/lab1_db"};
    pqxx::work txn{c};

    // 2. Execute SQL
    pqxx::row set =  txn.exec_params1("SELECT * FROM sets WHERE id=$1", id);
    txn.commit();

    // 3. Parse data (convert field from string to vector<int>)
    std::string rawJson = set[1].as<std::string>();
    auto jsonDataObj = nlohmann::json::parse(rawJson);

    return SetSerializer::from_json(jsonDataObj);
}
bool PostgresRepository::update(const IntegerSet& setUpdated, size_t id)
{
    // 1. Connect to the DB (Use the credentials from your docker-compose)
    pqxx::connection c{"postgresql://user:password@localhost:5432/lab1_db"};
    pqxx::work txn{c};

    // 2. Prepare data (convert to string)
    std::string jsonData = SetSerializer::to_json(setUpdated).dump();

    // 3. Execute SQL
    pqxx::result result = txn.exec_params0("UPDATE sets SET data=$1 WHERE id=$2", jsonData, id);
    bool wasUpdated = result.affected_rows();
    txn.commit();
    return wasUpdated;
}
bool PostgresRepository::remove(size_t id) {
    pqxx::connection c{"postgresql://user:password@localhost:5432/lab1_db"};
    pqxx::work txn{c};
    pqxx::result result = txn.exec_params0("DELETE FROM sets WHERE id=$1", id);
    bool wasUpdated = result.affected_rows();
    txn.commit();
    return wasUpdated;
}
