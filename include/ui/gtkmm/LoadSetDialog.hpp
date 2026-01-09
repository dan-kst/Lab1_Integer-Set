#ifndef LOAD_SET_DIALOG_HPP
#define LOAD_SET_DIALOG_HPP

#include <gtkmm.h>
#include "ui/common/WrapCore.hpp"

class LoadSetDialog : public Gtk::Dialog {
public:
    LoadSetDialog(Gtk::Window& parent, std::shared_ptr<WrapCore> core);

private:
    std::shared_ptr<WrapCore> core_;
// Widgets string values
    constexpr static std::string s_idColumnName_ = "ID";
    constexpr static std::string s_valueColumnName_ = "Value";
// Layout Widgets
    Gtk::Grid m_mainGrid;
    Gtk::ScrolledWindow m_setsWindow;
// Display
    Gtk::ColumnView m_dbView;
    Glib::RefPtr<Gtk::StringList> m_setsList;
    Glib::RefPtr<Gtk::SingleSelection> m_selection;
// Buttons
    Gtk::Button m_removeBtn;
// Helper functions
    void setupColumns();
    void setupGrid();
// ColumnView factories
    Glib::RefPtr<Gtk::SignalListItemFactory> createIdColumn();
    Glib::RefPtr<Gtk::SignalListItemFactory> createValueColumn();
};

#endif
