#ifndef INTEGER_SET_SERIALIZER_HPP
#define INTEGER_SET_SERIALIZER_HPP

#include "core/IntegerSet.hpp"
#include <nlohmann/json.hpp>

namespace SetStorage {
    nlohmann::json serialize(const IntegerSet& set);
    std::unique_ptr<IntegerSet> deserialize(const nlohmann::json& j);
}

#endif
