#include <gtkmm/application.h>
#include "./storage/PostgresRepository.hpp"
#include "./ui/common/WrapCore.hpp" // Use the renamed core
#include "./ui/gtkmm/SetMainWindow.hpp"

int main(int argc, char* argv[]) {
    // 1. Initialize dependencies
    auto repo = std::make_shared<PostgresRepository>();
    auto currentSet = std::make_shared<IntegerSet>();
    auto core = std::make_shared<WrapCore>(repo, currentSet);

    // 2. Create the GTK Application
    auto app = Gtk::Application::create("org.lab.integerset");

    // 3. Launch the Window
    return app->make_window_and_run<SetWindow>(argc, argv, core);
}
