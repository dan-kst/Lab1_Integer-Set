#ifndef SET_WINDOW_HPP
#define SET_WINDOW_HPP

#include <gtkmm.h>
#include "./ui/common/WrapCore.hpp"

class SetWindow : public Gtk::Window {
private:
    std::vector<std::string> localSetValues_;
    std::shared_ptr<WrapCore> core_;
// Widgets string values
    constexpr static std::string s_resultLabelValue_ = "No Results";
    constexpr static std::string s_valueColumnName_ = "Value";
// Layout Widgets
    Gtk::Grid m_mainGrid;
    Gtk::ScrolledWindow m_setsWindow;
    Gtk::Box m_localOperationsBox;
    Gtk::Box m_dbOperationsBox;
    Gtk::Box m_setOperationsBox;
// Buttons
    Gtk::Button m_createBtn;
    Gtk::Button m_removeBtn;
    Gtk::Button m_editBtn;
    Gtk::Button m_saveBtn;
    Gtk::Button m_loadBtn;
    Gtk::Button m_unionBtn;
    Gtk::Button m_interBtn;
    Gtk::Button m_diffBtn;
// Display widgets
    Gtk::Label m_resultLabel;
    Gtk::ColumnView m_setListView;
    Glib::RefPtr<Gtk::StringList> m_setValueStringList;
// Module Widgets
    Gtk::MessageDialog m_errorDialog;
    Gtk::MessageDialog m_infoDialog;
// ColumnView helper functions
    Glib::RefPtr<Gtk::SignalListItemFactory> createValueColumn();
    void refreshLocalList();

public:
    SetWindow(std::shared_ptr<WrapCore> core);
};

#endif
