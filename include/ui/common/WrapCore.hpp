#ifndef WRAP_CORE_HPP
#define WRAP_CORE_HPP

#include <memory>
#include <array>
#include <string>
#include <sstream>
#include <iostream>
#include "./core/IntegerSet.hpp"
#include "./storage/ISetRepository.hpp"
#include "./storage/IntegerSetSerializer.hpp"

enum class SetOperationType
{
    None,
    Union,
    Intersect,
    Difference
};
class WrapCore
{
private:
    std::shared_ptr<ISetRepository> repo_;
    std::shared_ptr<IntegerSet> currentSet_;
    size_t setId_;
public:
// Constructor
    WrapCore(std::shared_ptr<ISetRepository> repo, std::shared_ptr<IntegerSet> set);

// Helper functions
    size_t getId();
    void clearSet();
    std::string getSetJson();
    std::string getSetJson(size_t id);
    std::string parseJson(const std::string& jsonString);
    std::unique_ptr<IntegerSet> stringToSet(const std::string& jsonString);
// showSetsList "worker" function
    std::vector<size_t> getIdList();
// handleCreate "worker" function
    bool createSet(std::istringstream& input);
// handleUpdate "worker" function
    bool updateSet();
// handleDelete "worker" function
    bool removeSet(size_t id);
// handleSaveToDb "worker" function
    size_t saveSet();
// handleLoadFromDb "worker" functions
    bool loadSet(size_t id);
    bool loadSet(size_t id, IntegerSet& set);
// IntegerSet "worker" operations
    bool unionSets(size_t setOtherId);
    bool intersectSets(size_t setOtherId);
    bool differenceSets(size_t setOtherId);
    std::string performBatchOperation(const std::vector<std::string>& localSets, SetOperationType op);
};
#endif
