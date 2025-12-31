#ifndef CONSOLE_WRAP_UI_HPP
#define CONSOLE_WRAP_UI_HPP
#include <memory>
#include <array>
#include <sstream>
#include <iostream>
#include <limits>
#include "./core/IntegerSet.hpp"
#include "./storage/IntegerSetSerializer.hpp"
#include "./storage/ISetRepository.hpp"

class ConsoleWrapUI
{
private:
    constexpr static std::array<std::string, 8> mainMenuOptions_ =
    {
        "Exit",     //0
        "Create",   //1
        "Show",     //2
        "Edit",     //3
        "Operate",  //4
        "Save",     //5
        "Load",     //6
        "Bash-mode" //7
    };
    constexpr static std::array<std::string, 4> operateMenuOptions_ =
    {
        "Back",         //0
        "Unite",        //1
        "Intersect",    //2
        "Different"     //3
    };
    std::istringstream input_;
    size_t setId_;
// wrap own functions
    template<size_t N>
    void displayMenu(const std::array<std::string, N>& options);
    void showSetElements();
    void runCommandMode();
// handleCreate "worker" function
    void addSetElements(std::istringstream& input, IntegerSet& set);
// handleLoadFromDb "worker" function
    bool loadSet(int id, IntegerSet& set);
// handleUpdate "worker" function
    bool updateSet(std::istringstream& input);
// IntegerSet "worker" operations
    void unionSets(size_t setOtherId);
    void intersectSets(size_t setOtherId);
    void differenceSets(size_t setOtherId);

protected:
    std::shared_ptr<ISetRepository> repo_;
    std::unique_ptr<IntegerSet> currentSet_;
public:
// Constructor
    ConsoleWrapUI(std::shared_ptr<ISetRepository> repo);
// Launcher
    void Launch();
// CRUD operations
    void handleUpdate();
// Write into string stream
    bool handleRead(std::istream& input, std::istringstream& inputString);
// Write into an integer
    bool handleRead(std::istream& input, size_t& inputValue);
    void handleCreate();
// Operations with database
    void handleSaveToDb();
    void handleLoadFromDb();
// IntegerSet "UI" operations
    void handleUnion();
    void handleIntersect();
    void handleDifference();
};
#endif
