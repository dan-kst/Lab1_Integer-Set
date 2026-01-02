#include "./ui/console/ConsoleWrapCore.hpp"

// Constructor
ConsoleWrapCore::ConsoleWrapCore(std::shared_ptr<ISetRepository> repo, std::shared_ptr<IntegerSet> set)
: repo_(repo), currentSet_(set), setId_(0) {}

size_t ConsoleWrapCore::getId() { return setId_; }
std::vector<size_t> ConsoleWrapCore::getIdList()
{
    std::vector<size_t> idList;
    try
    {
        idList = repo_->getIdAll();
    }
    catch(std::exception& ex)
    {
        std::string error = "Couldn't load list due to internal error!\n";
        throw std::runtime_error(error + ex.what());
    }
    return idList;
}
void ConsoleWrapCore::clearSet()
{
    currentSet_->clear();
    setId_ = 0;
}
// handleCreate "worker" function
std::unique_ptr<IntegerSet> ConsoleWrapCore::createSet(std::istringstream& input)
{
    std::unique_ptr<IntegerSet> set = std::make_unique<IntegerSet>();
    int value;
    while (input >> value && (!input.fail()))
    {
        set->add(value);
    }
    if(input.fail() && !input.eof())
    {
        set.reset();
        std::cout << "Error! Set requires an integer arguments.\n";
    }
    input.clear();
    return set;
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
            setId_ = id;
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
