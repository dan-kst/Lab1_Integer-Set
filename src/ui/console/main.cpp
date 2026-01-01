#include "./storage/ISetRepository.hpp"
#include "./storage/PostgresRepository.hpp"
#include "./ui/console/ConsoleWrapUI.hpp"

int main() {
    std::shared_ptr<ISetRepository> repo = std::make_shared<PostgresRepository>();
    ConsoleWrapUI menu(repo);
    menu.LaunchBasicMode();
}
