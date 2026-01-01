#include "./ui/console/ConsoleWrapCore.hpp"

// Constructor
ConsoleWrapCore::ConsoleWrapCore(std::shared_ptr<ISetRepository> repo, std::shared_ptr<IntegerSet> set)
: repo_(repo), currentSet_(set) {}

// handleCreate "worker" function
void ConsoleWrapCore::addSetElements(std::istringstream& input)
{
    addSetElements(input, *currentSet_);
}
// handleCreate "worker" function
void ConsoleWrapCore::addSetElements(std::istringstream& input, IntegerSet& set)
{
    int value;
    while (input >> value && (!input.fail()))
    {
        set.add(value);
    }
    if(input.fail() && !input.eof())
    {
        std::cout << "Error! Set requires an integer arguments.\n";
    }
    input.clear();
}
// handleUpdate "worker" function
bool ConsoleWrapCore::updateSet()
{
    if(setId_)
    {
        repo_->update(*currentSet_, setId_);
        return true;
    }
    return false;
}
// handleSaveToDb "worker" function
size_t ConsoleWrapCore::saveSet()
{
    if(!setId_)
    {
        setId_ = repo_->save(*currentSet_);
        return setId_;
    }
    return 0;
}
// handleLoadFromDb "worker" functions
bool ConsoleWrapCore::loadSet(size_t id)
{
    return loadSet(id, *currentSet_);
}
bool ConsoleWrapCore::loadSet(size_t id, IntegerSet& set)
{
    try
    {
        auto loadedSet = repo_->load(id);
        if (loadedSet)
        {
            set = *loadedSet; // Rule of Three in action!
            return true;
        }
    }
    catch(std::exception& e)
    {
        throw std::range_error("Couldn't load the set with Id: " + id);
    }
    return false;
}
// IntegerSet "worker" operations
bool ConsoleWrapCore::unionSets(size_t setOtherId)
{
    bool success = false;
    auto setOther = std::make_unique<IntegerSet>();
    if(loadSet(setOtherId, *setOther))
    {
        *currentSet_ = *(currentSet_->unite(*setOther));
        success = true;
    }
    return success;
}
bool ConsoleWrapCore::intersectSets(size_t setOtherId)
{
    bool success = false;
    auto setOther = std::make_unique<IntegerSet>();
    if(loadSet(setOtherId, *setOther))
    {
        *currentSet_ = *(currentSet_->intersect(*setOther));
        success = true;
    }
    return success;
}
bool ConsoleWrapCore::differenceSets(size_t setOtherId)
{
    bool success = false;
    auto setOther = std::make_unique<IntegerSet>();
    if(loadSet(setOtherId, *setOther))
    {
        *currentSet_ = *(currentSet_->difference(*setOther));
        success = true;
    }
    return success;
}
