#ifndef CONSOLE_WRAP_CORE_HPP
#define CONSOLE_WRAP_CORE_HPP

#include <memory>
#include <array>
#include <string>
#include <sstream>
#include <iostream>
#include "./core/IntegerSet.hpp"
#include "./storage/ISetRepository.hpp"
#include "./storage/IntegerSetSerializer.hpp"

class ConsoleWrapCore
{
private:
    std::shared_ptr<ISetRepository> repo_;
    std::shared_ptr<IntegerSet> currentSet_;
    size_t setId_;
public:
// Constructor
    ConsoleWrapCore(std::shared_ptr<ISetRepository> repo, std::shared_ptr<IntegerSet> set);

    size_t getId();
    std::vector<size_t> getIdList();
    void clearSet();
// handleCreate "worker" function
    std::unique_ptr<IntegerSet> createSet(std::istringstream& input);
// handleUpdate "worker" function
    bool updateSet();
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
