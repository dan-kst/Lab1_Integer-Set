#include "./ui/console/ConsoleWrap.hpp"

// wrap own methods
void ConsoleWrap::showSetsList()
{
    try
    {
        std::vector<size_t> idList = core_->getIdList();
        if(idList.size() > 0)
        {
            std::cout << "Id of available sets to load:" << std::endl;
            for(auto id : idList)
            {
                std::cout << id << std::endl;
            }
        }
        else
        {
            std::cout << "No available sets to load:" << std::endl;
        }
    }
    catch(std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}
void ConsoleWrap::showSetElements()
{
    std::string elements = core_->getSetString();

    if (elements.empty())
    {
        std::cout << "Current Set is empty." << std::endl;
    }
    else
    {
        std::cout << "Current Set: " << elements << std::endl;
    }
}
// Constructor
ConsoleWrap::ConsoleWrap(std::shared_ptr<ISetRepository> repo)
{
    currentSet_ = std::make_shared<IntegerSet>();
    core_ = std::make_unique<WrapCore>(repo, currentSet_);
}
// Launcher
void ConsoleWrap::LaunchBasicMode()
{
    size_t choice = mainMenuOptions_.size();
    while(choice)
    {
        displayMenu(mainMenuOptions_);
        if(handleRead(std::cin, choice))
        {
            switch(choice)
            {
            // Exit
                case 0:
                break;
            // Create
                case 1:
                    std::cout << "Enter integers separated by spaces or leave prompt empty to cancel: ";
                    handleCreate(std::cin);
                break;
            // Show
                case 2:
                    showSetElements();
                break;
            // Edit
                case 3:
                    std::cout << core_->getId() << std::endl;
                    if(core_->getId() != 0)
                    {
                        showSetElements();
                        std::cout << "Enter integers separated by spaces or leave prompt empty to cancel: ";
                        handleUpdate(std::cin);
                    }
                    else
                    {
                        std::cout << "Cannot update local set\n";
                    }
                break;
            // Operate
                case 4:
                    choice = std::string::npos;
                    while(choice)
                    {
                        displayMenu(operateMenuOptions_);
                        if(handleRead(std::cin, choice))
                        {
                            switch(choice)
                            {
                            // Union
                                case 1:
                                    std::cout << "Enter the ID of the set to union with: ";
                                    handleUnion(std::cin);
                                break;
                            // Intersect
                                case 2:
                                    std::cout << "Enter the ID of the set to intersect with: ";
                                    handleIntersect(std::cin);
                                break;
                            // Difference
                                case 3:
                                    std::cout << "Enter the ID of the set to difference with: ";
                                    handleDifference(std::cin);
                                break;
                                default:
                                    std::cout << "Entered unknown menu index!\n ";
                                break;
                            }
                        }
                        else
                        {
                            std::cout << "Entered wrong value!\n ";
                        }
                    }
                    choice = mainMenuOptions_.size();
                break;
            // Save
                case 5:
                    handleSaveToDb();
                break;
            // Load
                case 6:
                    showSetsList();
                    std::cout << "\nEnter the ID of the set to load: ";
                    handleLoadFromDb(std::cin);
                break;
            // Remove
                case 7:
                    showSetsList();
                    showSetElements();
                    std::cout << "\nEnter the ID of the set to remove: ";
                    handleDelete(std::cin);
                break;
            // Bash-mode
                case 8:
                    LaunchAdvancedMode();
                break;
                default:
                    std::cout << "Entered unknown menu index!\n ";
                break;
            }
        }
        else
        {
            std::cout << "Entered wrong value!\n ";
        }
    }
}
// Bash-like mode
void ConsoleWrap::LaunchAdvancedMode()
{
    std::string input;
    std::cout << "Entering Command Mode (type 'exit' to return to menu)\n";
    auto currentSetCopy = std::make_unique<IntegerSet>();
    if(currentSet_->size() > 0)
        *currentSetCopy = *currentSet_;
    bool hasError = false;
    do
    {
        if(!input.empty())
        {
            std::istringstream commandLine;
            commandLine.str(input);
            commandLine.clear();
            std::string action;
            while (std::getline(commandLine >> std::ws, input, '&'))
            {
                if(!input.empty())
                {
                    std::istringstream commandInput(input);
                    std::getline(commandInput >> std::ws, action, ' ');
                    if (action == "create")
                    {
                        handleCreate(commandInput);
                    }
                    else if (action == "show")
                    {
                        showSetElements();
                    }
                    else if (action == "edit")
                    {
                        if(!core_->getId())
                        {
                            handleUpdate(commandInput);
                        }
                        else
                        {
                            std::cout << "Cannot update local set\n";
                        }
                    }
                    else if (action == "save")
                    {
                        handleSaveToDb();
                    }
                    else if (action == "list")
                    {
                        showSetsList();
                    }
                    else if (action == "load")
                    {
                        handleLoadFromDb(commandInput);
                    }
                    else if (action == "union")
                    {
                        handleUnion(commandInput);
                    }
                    else if (action == "intersect")
                    {
                        handleIntersect(commandInput);
                    }
                    else if (action == "difference")
                    {
                        handleDifference(commandInput);
                    }
                    else
                    {
                        std::cout << "Error! Unknown command " << action << std::endl;
                    }
                }
            }
        }
        std::cout << "$ ";
    }while (std::getline(std::cin >> std::noskipws, input) && input != "exit" && !hasError);
    
    if(hasError)
    {
        *currentSet_ = *currentSetCopy;
    }
}
// CRUD operations
void ConsoleWrap::handleCreate(std::istream& inputStream)
{
    std::istringstream inputLine;
    if(handleRead(inputStream, inputLine))
    {
        std::unique_ptr<IntegerSet> set = core_->createSet(inputLine);
        if(set)
        {
            core_->clearSet();
            *currentSet_ = *set;
            std::cout << "Added elements. New size: " << currentSet_->size() << std::endl;
        }
        else
        {
            std::cout << "Failed to create a set.\n";
        }
    }
    else
    {
        std::cout << "Creation have been cancelled.\n";
    }
}
// Write into string stream
bool ConsoleWrap::handleRead(std::istream& input, std::istringstream& inputString)
{
    std::string inputStr;
    std::getline(input >> std::noskipws, inputStr, '\n');
    inputString.str(inputStr);
    return !inputStr.empty() && inputStr != "\n";
}
// Write into an integer
bool ConsoleWrap::handleRead(std::istream& input, size_t& inputValue)
{
    std::istringstream inputString;
    bool readIntSuccess = handleRead(input, inputString);
    if(readIntSuccess)
    {
        size_t inputValueCopy = inputValue;
        inputString >> inputValue;
        readIntSuccess = !inputString.fail();
        if(!readIntSuccess)
        {
            inputString.clear();
            std::cout << "Error! Integer value required.\n";
            inputValue = inputValueCopy;
        }
    }
    return readIntSuccess;
}
void ConsoleWrap::handleUpdate(std::istream& inputStream)
{
    std::istringstream inputLine;
    if(handleRead(inputStream, inputLine))
    {
        std::unique_ptr<IntegerSet> set = core_->createSet(inputLine);
        if(set)
        {
            *currentSet_ = *set;
            std::cout << "Set was updated successfully!\n" << std::endl;
        }
        else
        {
            std::cout << "Failed to update a set.\n";
        }
    }
    else
    {
        std::cout << "Editing have been cancelled.\n";
    }
}
void ConsoleWrap::handleDelete(std::istream& inputStream)
{
    size_t id;
    try
    {
        if(handleRead(inputStream, id) && core_->removeSet(id))
        {
            std::cout << "Set with ID '" << id << "' was removed successfully!" << std::endl;
        }
        else
        {
            std::cout << "Failed to remove the set!\n" << std::endl;
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Error! Cannot remove set!\n" << e.what() << std::endl;
    }
}
// Operations with database
void ConsoleWrap::handleSaveToDb()
{
    try
    {
        size_t id = core_->saveSet();
        if(id)
        {
            std::cout << "New set saved successfully with ID: " << id << std::endl;
        }
        else if(core_->updateSet())
        {
            std::cout << "Set was updated successfully!" << std::endl;
        }
        else
        {
            std::cout << "Saving/updating operation was failed!"  << std::endl;
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Error! Cannot save/update set: " << e.what() << std::endl;
    }
}
void ConsoleWrap::handleLoadFromDb(std::istream& inputStream)
{
    size_t id;
    try
    {
        if(handleRead(inputStream, id) && core_->loadSet(id))
        {
            std::cout << "Set with ID '" << id << "' was loaded successfully!" << std::endl;
        }
        else
        {
            std::cout << "Failed to load the set!\n" << std::endl;
        }
    }
    catch(std::exception& ex)
    {
        std::cout <<  ex.what() << std::endl;
    }
}
// IntegerSet "UI" operations
void ConsoleWrap::handleUnion(std::istream& inputStream)
{
    size_t id;
    if(handleRead(inputStream, id) && core_->unionSets(id))
    {
        std::cout << "Union operation was successful!\n\n";
    }
    else
    {
        std::cout << "Failed to union sets.\n";
    }
}
void ConsoleWrap::handleIntersect(std::istream& inputStream)
{
    size_t id;
    if(handleRead(inputStream, id) && core_->intersectSets(id))
    {
        std::cout << "Intersect operation was successful!\n\n";
    }
    else
    {
        std::cout << "Failed to intersect sets.\n";
    }
}
void ConsoleWrap::handleDifference(std::istream& inputStream)
{
    size_t id;
    if(handleRead(inputStream, id) && core_->differenceSets(id))
    {
        std::cout << "Difference  operation was successful!\n\n";
    }
    else
    {
        std::cout << "Failed to difference sets.\n";
    }
}
