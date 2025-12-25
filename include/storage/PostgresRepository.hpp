#ifndef POSTGRES_REPOSITORY_HPP
#define POSTGRES_REPOSITORY_HPP

#include <pqxx/pqxx>
#include "ISetRepository.hpp" 

class PostgresRepository : public ISetRepository{
public:
    virtual ~PostgresRepository();
    size_t save(const IntegerSet& set) override;
    std::unique_ptr<IntegerSet> load(size_t id) override;
};

#endif
