#include <iostream>
#include <sstream>
#include "./storage/ISetRepository.hpp"
#include "./storage/PostgresRepository.hpp"
#include "./core/IntegerSet.hpp"

void printMainMenu()
{
    std::cout << "\n--- Integer Set Lab ---\n"
          << "1. Add Elements\n"
          << "2. Show Set\n"
          << "3. Save\n"
          << "4. Load\n"
          << "5. Enter Command Mode (Bash-like)\n"
          << "0. Exit\n"
          << "Choice: ";
}
// Version B: The "Worker" function
void handleAdd(IntegerSet& set, std::stringstream& ss)
{
    int value;
    while (ss >> value)
    {
        set.add(value);
    }
    std::cout << "Added elements. New size: " << set.size() << std::endl;
}
// Version A: The "UI" function for the Integer Menu
void handleAdd(IntegerSet& set)
{
    std::cout << "Enter integers separated by spaces: ";
    std::string line;
    std::getline(std::cin >> std::ws, line);
    
    std::stringstream ss(line);
    handleAdd(set, ss); // Delegate to the worker
}

void displaySet(const IntegerSet& set)
{
    std::vector<int> elements = set.toVector(); // Get data from the model
    
    if (elements.empty())
    {
        std::cout << "The set is currently empty. 📭" << std::endl;
        return;
    }

    std::cout << "Current Set: { ";
    for (size_t i = 0; i < elements.size(); ++i)
    {
        std::cout << elements[i] << (i == elements.size() - 1 ? "" : ", ");
    }
    std::cout << " }" << std::endl;
}

void handleDatabase(IntegerSet& currentSet, ISetRepository& repo, bool isSave)
{
    if (isSave)
    {
        size_t id = repo.save(currentSet);
        std::cout << "Set saved successfully with ID: " << id << std::endl;
    }
    else
    {
        int id;
        std::cout << "Enter the ID of the set to load: ";
        if (std::cin >> id)
        {
            auto loadedSet = repo.load(id);
            if (loadedSet)
            {
                currentSet = *loadedSet; // Rule of Three in action!
                std::cout << "Set loaded!" << std::endl;
            }
        }
    }
}
int main() {
    std::unique_ptr<ISetRepository> repo = std::make_unique<PostgresRepository>();
    IntegerSet currentSet;

    int choice = -1;
    while (choice != 0) {
        printMainMenu();
        std::cin >> choice;
        switch(choice)
        {
            case 1:
                handleAdd(currentSet);
                break;
            case 2:
                displaySet(currentSet);
                break;
            case 3:
                handleDatabase(currentSet, *repo, true);
                break;
            case 4:
                handleDatabase(currentSet, *repo, false);
                break;
            case 5:
                break;
        }
    }
}
