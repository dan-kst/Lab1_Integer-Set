#include "./ui/common/WrapCore.hpp"

// Constructor
WrapCore::WrapCore(std::shared_ptr<ISetRepository> repo, std::shared_ptr<IntegerSet> set)
: repo_(repo), currentSet_(set), setId_(0) {}

size_t WrapCore::getId() { return setId_; }
// showSetsList "worker" function
std::vector<size_t> WrapCore::getIdList()
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
void WrapCore::clearSet()
{
    currentSet_->clear();
    setId_ = 0;
}
std::string WrapCore::getSetString()
{
    if(currentSet_->size() > 0)
    {
        return SetSerializer::to_json(*currentSet_).at(SetSerializer::valueName).dump();
    }
    return "";
}
std::string WrapCore::getSetString(size_t id)
{
    auto tempSet = repo_->load(id);
    if (tempSet) {
        return SetSerializer::to_json(*tempSet).at(SetSerializer::valueName).dump();
    }
    return "{}";
}
// handleCreate "worker" function
bool WrapCore::createSet(std::istringstream& input)
{
    std::unique_ptr<IntegerSet> set = std::make_unique<IntegerSet>();
    int value;
    while (input >> value && (!input.fail()))
    {
        set->add(value);
    }
    if((input.fail() && !input.eof()) || set->size() == 0)
    {
        input.clear();
        set.reset();
        return false;
    }
    else
    {
        *currentSet_ = *set;
        return true;
    }
}
// handleUpdate "worker" function
bool WrapCore::updateSet()
{
    if(setId_)
    {
        repo_->update(*currentSet_, setId_);
        return true;
    }
    return false;
}
bool WrapCore::removeSet(size_t id)
{
    bool isRemoved = false;
    try
    {
        if (repo_->remove(id))
        {
            if(id == setId_)
            {
                clearSet();
            }
            isRemoved = true;
        }
    }
    catch(std::exception& ex)
    {
        std::string errorMassage = "Couldn't remove a set with this Id.\n";
        throw std::range_error(errorMassage + ex.what());
    }
    return isRemoved;
}
// handleSaveToDb "worker" function
size_t WrapCore::saveSet()
{
    if(!setId_ && currentSet_->size() > 0)
    {
        setId_ = repo_->save(*currentSet_);
        return setId_;
    }
    return 0;
}
// handleLoadFromDb "worker" functions
bool WrapCore::loadSet(size_t id)
{
    return loadSet(id, *currentSet_);
}
bool WrapCore::loadSet(size_t id, IntegerSet& set)
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
    catch(std::exception& ex)
    {
        std::string errorMassage = "Couldn't load a set with this Id\n";
        throw std::range_error(errorMassage + ex.what());
    }
    return false;
}
// IntegerSet "worker" operations
bool WrapCore::unionSets(size_t setOtherId)
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
bool WrapCore::intersectSets(size_t setOtherId)
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
bool WrapCore::differenceSets(size_t setOtherId)
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
