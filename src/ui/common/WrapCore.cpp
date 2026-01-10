#include "./ui/common/WrapCore.hpp"

// Constructor
WrapCore::WrapCore(std::shared_ptr<ISetRepository> repo, std::shared_ptr<IntegerSet> set)
: repo_(repo), currentSet_(set), setId_(0) {}

size_t WrapCore::getId() { return setId_; }
void WrapCore::clearSet()
{
    currentSet_->clear();
    setId_ = 0;
}
std::string WrapCore::getSetJson()
{
    if(currentSet_->size() > 0)
    {
        return SetSerializer::to_json(*currentSet_).at(SetSerializer::valueName).dump();
    }
    return "";
}
std::string WrapCore::getSetJson(size_t id)
{
    auto tempSet = repo_->load(id);
    if (tempSet) {
        return SetSerializer::to_json(*tempSet).at(SetSerializer::valueName).dump();
    }
    return "{}";
}
std::string WrapCore::parseJson(const std::string& jsonString)
{
    auto j = nlohmann::json::parse(jsonString);
    std::string spaceString;
    for (auto& item : j) {
        if (!spaceString.empty()) spaceString += " ";
        spaceString += item.dump();
    }
    return spaceString;
}
std::unique_ptr<IntegerSet> WrapCore::stringToSet(const std::string& jsonString)
{
    try
    {
        auto j = nlohmann::json::parse(jsonString);
        // If the input is a raw array like [1,2,3], wrap it for the serializer
        if (j.is_array()) {
            nlohmann::json wrapper = {{SetSerializer::valueName, j}};
            return SetSerializer::from_json(wrapper);
        }
        return SetSerializer::from_json(j);
    }
    catch (...)
    {
        return std::make_unique<IntegerSet>();
    }
}
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
        setId_ = 0;
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
size_t WrapCore::saveSet(const IntegerSet& set)
{
    if(set.size() > 0)
    {
        setId_ = repo_->save(set);
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
std::string WrapCore::performBatchOperation(const std::vector<std::string>& localSets, SetOperationType op)
{
    if (localSets.empty() || op == SetOperationType::None) return "No Data";

    auto result = stringToSet(localSets[0]);
    auto nextSet = std::make_unique<IntegerSet>();
    for (size_t i = 1; i < localSets.size(); i++)
    {
        *nextSet = *(stringToSet(localSets[i]));
        switch (op)
        {
            case SetOperationType::None:
                break;
            case SetOperationType::Union:
                *result = *(result->unite(*nextSet));
                break;
            case SetOperationType::Intersect:
                result = result->intersect(*nextSet);
                break;
            case SetOperationType::Difference:
                result = result->difference(*nextSet);
                break;
        }
    }
    return SetSerializer::to_json(*result).at(SetSerializer::valueName).dump();
}
