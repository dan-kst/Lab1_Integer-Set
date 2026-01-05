#ifndef SET_WINDOW_HPP
#define SET_WINDOW_HPP

#include <gtkmm.h>
#include "./ui/common/WrapCore.hpp"

class SetWindow : public Gtk::Window {
private:
    constexpr static std::string lblStatusString = "Current Set: ";
    std::shared_ptr<WrapCore> core_;

    // Layout Widgets
    Gtk::Box m_mainBox;
    Gtk::Label m_lblStatus;
    Gtk::Entry m_entryInput;
    Gtk::Box m_buttonBox;

    // Module Widgets
    Gtk::MessageDialog m_errorDialog;
    Gtk::MessageDialog m_infoDialog;

    // Buttons
    Gtk::Button m_btnAdd;
    Gtk::Button m_btnSave;
    Gtk::Button m_btnLoad;

    void on_add_clicked();

public:
    SetWindow(std::shared_ptr<WrapCore> core);
};

#endif
