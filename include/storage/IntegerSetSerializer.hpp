#ifndef INTEGER_SET_SERIALIZER_HPP
#define INTEGER_SET_SERIALIZER_HPP

#include "core/IntegerSet.hpp"
#include <nlohmann/json.hpp>

namespace SetSerializer {
    nlohmann::json to_json(const IntegerSet& set);
    std::unique_ptr<IntegerSet> from_json(const nlohmann::json& j);
}

#endif
