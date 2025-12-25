#ifndef ISETREPOSITORY_HPP
#define ISETREPOSITORY_HPP
#include "core/IntegerSet.hpp"

class ISetRepository {
public:
    virtual ~ISetRepository() = default;
    virtual void save(const IntegerSet& set, int id) = 0;
    virtual std::unique_ptr<IntegerSet> load(int id) = 0;
};
#endif
