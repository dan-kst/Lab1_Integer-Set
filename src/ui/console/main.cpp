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
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
        }
    }
}
