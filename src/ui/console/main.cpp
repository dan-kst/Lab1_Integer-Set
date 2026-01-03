#include "./storage/ISetRepository.hpp"
#include "./storage/PostgresRepository.hpp"
#include "./ui/console/ConsoleWrap.hpp"

int main() {
    std::shared_ptr<ISetRepository> repo = std::make_shared<PostgresRepository>();
    ConsoleWrap menu(repo);
    menu.LaunchBasicMode();
}
