#include "./storage/IntegerSetSerializer.hpp"

namespace SetSerializer{
    nlohmann::json to_json(const IntegerSet& set)
    {
        return nlohmann::json{valueName, set.toVector()};
    }
    std::unique_ptr<IntegerSet> from_json(const nlohmann::json& j)
    {
        std::unique_ptr<IntegerSet> set = std::make_unique<IntegerSet>();
        auto vec = j.at(SetSerializer::valueName).get<std::vector<int>>();
        for(int x : vec) 
        {
            set->add(x);
        }
        return set;
    }
}
