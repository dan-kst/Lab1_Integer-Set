#ifndef ISETREPOSITORY_HPP
#define ISETREPOSITORY_HPP
#include "core/IntegerSet.hpp"

class ISetRepository {
public:
    virtual ~ISetRepository() = default;
    virtual size_t save(const IntegerSet& set) = 0;
    virtual std::unique_ptr<IntegerSet> load(size_t id) = 0;
};
#endif
