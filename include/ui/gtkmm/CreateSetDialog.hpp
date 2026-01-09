#ifndef CREATE_SET_DIALOG_HPP
#define CREATE_SET_DIALOG_HPP

#include <gtkmm.h>

class CreateSetDialog : public Gtk::Dialog
{
public:
    CreateSetDialog(Gtk::Window& parent);
    std::string getInputValue() const { return m_entry.get_text(); }

private:
    Gtk::Box m_contentBox;
    Gtk::Label m_label;
    Gtk::Entry m_entry;
};

#endif
