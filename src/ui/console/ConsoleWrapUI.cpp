#include "./ui/console/ConsoleWrapUI.hpp"

// wrap own functions
template<size_t N>
void ConsoleWrapUI::displayMenu(const std::array<std::string, N>& options)
{
    size_t optionsSize = N;
    std::string suffix(" set");
    for(size_t i = 1; i < optionsSize; i++)
    {
        std::cout << std::to_string(i) << ". "  << options[i] << suffix << std::endl;
    }
    std::cout << std::to_string(0) << ". "  << options[0] << std::endl;
    std::cout << "Enter integer to choose: ";
}
void ConsoleWrapUI::showSetElements()
{
    std::string elements = SetSerializer::to_json(*currentSet_).at(1).dump();

    if (elements.empty())
    {
        std::cout << "The set is currently empty." << std::endl;
    }
    else
    {
        std::cout << "Current Set: " << elements << std::endl;
    }
}
// handleCreate "worker" function
void ConsoleWrapUI::addSetElements(std::istringstream& input, IntegerSet& set)
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
    else
    {
        input.clear();
    }
}
// handleLoadFromDb "worker" function
bool ConsoleWrapUI::loadSet(int id, IntegerSet& set)
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
        std::cout << "Error! Cannot load entry: " << e.what() << std::endl;
    }
    return false;
}
// IntegerSet "worker" operations
void ConsoleWrapUI::unionSets(size_t setOtherId)
{
    auto setOther = std::make_unique<IntegerSet>();
    if(loadSet(setOtherId, *setOther))
    {
        *currentSet_ = *(currentSet_->unite(*setOther));
    }
}
void ConsoleWrapUI::intersectSets(size_t setOtherId)
{
    auto setOther = std::make_unique<IntegerSet>();
    if(loadSet(setOtherId, *setOther))
    {
        *currentSet_ = *(currentSet_->intersect(*setOther));
    }
}
void ConsoleWrapUI::differenceSets(size_t setOtherId)
{
    auto setOther = std::make_unique<IntegerSet>();
    if(loadSet(setOtherId, *setOther))
    {
        *currentSet_ = *(currentSet_->difference(*setOther));
    }
}



// Constructor
ConsoleWrapUI::ConsoleWrapUI(std::shared_ptr<ISetRepository> repo) : setId_(0), repo_(repo) 
{
    currentSet_ = std::make_unique<IntegerSet>();
}
// Launcher
void ConsoleWrapUI::Launch()
{
    size_t choice = std::string::npos;
    while(choice)
    {
        displayMenu(mainMenuOptions_);
        handleRead(std::cin, choice);
        switch(choice)
        {
        // Exit
            case 0:
                break;
        // Create
            case 1:
                handleCreate();
                break;
        // Show
            case 2:
                showSetElements();
                break;
        // Edit
            case 3:
                handleUpdate();
                break;
                // Operate
            case 4:
                choice = std::string::npos;
                while(choice)
                {
                    displayMenu(operateMenuOptions_);
                    handleRead(std::cin, choice);
                    switch(choice)
                    {
                        case 1:
                            handleUnion();
                            choice = 0;
                            break;
                        case 2:
                            handleIntersect();
                            choice = 0;
                            break;
                        case 3:
                            handleDifference();
                            choice = 0;
                            break;
                        default:
                            std::cout << "Entered wrong value!\n ";
                            break;
                    }
                }
                choice = 4;
                break;
        // Save
            case 5:
                handleSaveToDb();
                break;
        // Load
            case 6:
                handleLoadFromDb();
                break;
            default:
                std::cout << "Entered wrong value!\n ";
                break;
        }
    }
}
// CRUD operations
void ConsoleWrapUI::handleUpdate()
{
    if(setId_)
    {
        std::unique_ptr<IntegerSet> editSet = std::make_unique<IntegerSet>();
        showSetElements();
        std::cout << "Enter integers separated by spaces or leave prompt empty to: ";
        handleRead(std::cin, input_);
        addSetElements(input_, *editSet);
        if(editSet->size() > 0)
        {
            *currentSet_ = *editSet;
            std::cout << "Set have been updated! Current size: " << currentSet_->size() << std::endl;
        }
        else
        {
            std::cout << "Editing have been cancelled\n";
        }
    }
    else
    {
        std::cout << "Cannot edit locally created set.\n";
    }
}
// Write into string stream
void ConsoleWrapUI::handleRead(std::istream& input, std::istringstream& inputString)
{
    std::string inputStr;
    std::getline(input >> std::noskipws, inputStr);
    inputString.str(inputStr);
}
// Write into an integer
void ConsoleWrapUI::handleRead(std::istream& input, size_t& inputValue)
{
    input >> inputValue;
    if(input.fail())
    {
        input.clear();
        std::cout << "Error! Integer value required.\n";
        inputValue = std::string::npos;
    }
    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void ConsoleWrapUI::handleCreate()
{
    std::cout << "Enter integers separated by spaces: ";
    handleRead(std::cin, input_);
    addSetElements(input_, *currentSet_);
    setId_ = 0;
    std::cout << "Added elements. New size: " << currentSet_->size() << std::endl;
}
// Operations with database
void ConsoleWrapUI::handleSaveToDb()
{
    try
    {
        std::cout << "Try to save/update with ID '" << setId_ << "' ..." << std::endl;
        if(setId_)
        {
            repo_->update(*currentSet_, setId_);
            std::cout << "Set updated successfully with ID: " << setId_ << std::endl;
        }
        else
        {
            setId_ = repo_->save(*currentSet_);
            std::cout << "New set saved successfully with ID: " << setId_ << std::endl;
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Error! Cannot save/update entry: " << e.what() << std::endl;
    }
}
void ConsoleWrapUI::handleLoadFromDb()
{
    size_t id = std::string::npos;
    std::cout << "Enter the ID of the set to load: ";
    handleRead(std::cin, id);
    input_.str(std::to_string(id));
    input_.seekg(0);
    if(loadSet(id, *currentSet_))
    {
        setId_ = id;
        std::cout << "\nSet with ID '" << setId_ << "' was loaded successfully!" << std::endl;
    }
}
// IntegerSet "UI" operations
void ConsoleWrapUI::handleUnion()
{
    size_t id = 0;
    std::cout << "Enter the ID of the set to unite: ";
    handleRead(std::cin, id);
    if(id)
    {
        unionSets(id);
        std::cout << "Union operation was successful!\n\n";
    }
    else
    {
        std::cout << "Cannot unite sets.\n";
    }
}
void ConsoleWrapUI::handleIntersect()
{
    size_t id = 0;
    std::cout << "Enter the ID of the set to intersect: ";
    handleRead(std::cin, id);
    if(id)
    {
        intersectSets(id);
        std::cout << "Intersect operation was successful!\n\n";
    }
    else
    {
        std::cout << "Cannot intersect sets.\n";
    }
}
void ConsoleWrapUI::handleDifference()
{
    size_t id = 0;
    std::cout << "Enter the ID of the set to difference: ";
    handleRead(std::cin, id);
    if(id)
    {
        differenceSets(id);
        std::cout << "Difference operation was successful!\n\n";
    }
    else
    {
        std::cout << "Cannot difference sets.\n";
    }
}
