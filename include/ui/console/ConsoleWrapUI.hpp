#ifndef CONSOLE_WRAP_UI_HPP
#define CONSOLE_WRAP_UI_HPP

#include <memory>
#include <array>
#include <string>
#include <sstream>
#include <iostream>
#include <limits>
#include "./core/IntegerSet.hpp"
#include "./ui/console/ConsoleWrapCore.hpp"

class ConsoleWrapUI
{
private:
    constexpr static std::array<std::string, 8> mainMenuOptions_ =
    {
        "Exit",                 //0
        "Create",               //1
        "Show",                 //2
        "Edit",                 //3
        "Operate Sets",         //4
        "Save",                 //5
        "Load",                 //6
        "Bash-mode"             //7
    };
    constexpr static std::array<std::string, 4> operateMenuOptions_ =
    {
        "Back",         //0
        "Union",        //1
        "Intersect",    //2
        "Difference"    //3
    };
    std::unique_ptr<ConsoleWrapCore> core_;
    std::shared_ptr<IntegerSet> currentSet_;
// wrap own methods
    void showSetElements();
    template<size_t N>
    void displayMenu(const std::array<std::string, N>& options)
    {
        size_t optionsSize = N;
        std::string suffix(" set");
        std::cout << std::endl;
        for(size_t i = 1; i < optionsSize; i++)
        {
            std::cout << std::to_string(i) << ". "  << options[i] << suffix << std::endl;
        }
        std::cout << std::to_string(0) << ". "  << options[0] << std::endl;
        std::cout << "Enter integer to choose: ";
    }
public:
// Constructor
    ConsoleWrapUI(std::shared_ptr<ISetRepository> repo);
// Launcher
    void LaunchBasicMode();
    void LaunchAdvancedMode();
// CRUD operations
    void handleCreate(std::istream& inputStream);
    // Write into string stream
    bool handleRead(std::istream& input, std::istringstream& inputString);
    // Write into an integer
    bool handleRead(std::istream& input, size_t& inputValue);
    void handleUpdate(std::istream& inputStream);
// Operations with database
    void handleSaveToDb();
    void handleLoadFromDb(std::istream& inputStream);
// IntegerSet "UI" operations
    void handleUnion(std::istream& inputStream);
    void handleIntersect(std::istream& inputStream);
    void handleDifference(std::istream& inputStream);
};
#endif
