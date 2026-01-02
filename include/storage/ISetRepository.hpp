#ifndef I_SET_REPOSITORY_HPP
#define I_SET_REPOSITORY_HPP
#include "./core/IntegerSet.hpp"

class ISetRepository
{
public:
    virtual ~ISetRepository() = default;
    virtual size_t save(const IntegerSet& set) = 0;
    virtual std::unique_ptr<IntegerSet> load(size_t id) = 0;
    virtual bool update(const IntegerSet& set, size_t id) = 0;
    virtual bool remove(size_t id) = 0;
    virtual std::vector<size_t> getIdAll() = 0;
};
#endif
