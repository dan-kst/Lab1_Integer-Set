#include "./storage/IntegerSetSerializer.hpp"

void to_json(nlohmann::json& j, const IntegerSet& s) {
    std::vector<int> elementsVec;
    // We can't pass a raw pointer to JSON, so we convert to a vector first
    for(size_t i = 0; i < s.size(); i++) {
        // You'll need a way to access elements by index or a getter
        // Let's assume you add a getElements() or similar
    }
    j = nlohmann::json{{"elements", elementsVec}};
}

void from_json(const nlohmann::json& j, IntegerSet& s) {
    auto vec = j.at("elements").get<std::vector<int>>();
    for(int x : vec) {
        s.add(x);
    }
}
