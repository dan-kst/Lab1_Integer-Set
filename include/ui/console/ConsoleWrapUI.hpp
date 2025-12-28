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
    constexpr static std::array<std::string, 3> mainMenuOptions_ =
    {
        "Exit",     //0
        "Create",   //1
        "Show",     //2
    };
    std::istringstream input_;
// wrap own functions
    template<size_t N>
    void displayMenu(const std::array<std::string, N>& options);
    void showSetElements();
// handleCreate "worker" function
    void addSetElements(std::istringstream& input);

protected:
    std::shared_ptr<ISetRepository> repo_;
    std::unique_ptr<IntegerSet> currentSet_;
public:
// Constructor
    ConsoleWrapUI(std::shared_ptr<ISetRepository> repo);
// Launcher
    void Launch();
// CRUD operations
// Write into string stream
    void handleRead(std::istream& input);
// Write into an integer
    void handleRead(std::istream& input, size_t& inputValue);
    void handleCreate();
};
#endif
