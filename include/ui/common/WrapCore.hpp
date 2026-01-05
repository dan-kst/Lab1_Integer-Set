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

class WrapCore
{
private:
    std::shared_ptr<ISetRepository> repo_;
    std::shared_ptr<IntegerSet> currentSet_;
    size_t setId_;
public:
// Constructor
    WrapCore(std::shared_ptr<ISetRepository> repo, std::shared_ptr<IntegerSet> set);

    size_t getId();
// showSetsList "worker" function
    std::vector<size_t> getIdList();
    void clearSet();
    std::string getSetString();
// handleCreate "worker" function
    std::unique_ptr<IntegerSet> createSet(std::istringstream& input);
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
};
#endif
