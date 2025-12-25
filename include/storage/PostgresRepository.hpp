#ifndef POSTGRES_REPOSITORY_HPP
#define POSTGRES_REPOSITORY_HPP

#include <pqxx/pqxx>
#include "ISetRepository.hpp" 

class PostgresRepository : public ISetRepository{
public:
    virtual ~PostgresRepository();
    void save(const IntegerSet& set, int id) override;
    std::unique_ptr<IntegerSet> load(int id) override;
};

#endif
