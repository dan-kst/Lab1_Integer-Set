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



// Constructor
ConsoleWrapUI::ConsoleWrapUI(std::shared_ptr<ISetRepository> repo) : repo_(repo)
{
    currentSet_ = std::make_unique<IntegerSet>();
}
// Launcher
void ConsoleWrapUI::Launch()
{
    size_t choice = std::string::npos;
    while(choice != 0)
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
            default:
                std::cout << "Entered wrong value!\n ";
                break;
        }
    }
}
// CRUD operations
void ConsoleWrapUI::handleUpdate()
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
    std::cout << "Editing have been cancelled\n";
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
    std::cout << "Added elements. New size: " << currentSet_->size() << std::endl;
}
