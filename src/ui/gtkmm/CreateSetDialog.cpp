#include "ui/gtkmm/CreateSetDialog.hpp"

CreateSetDialog::CreateSetDialog(Gtk::Window& parent)
    :
    m_contentBox(Gtk::Orientation::VERTICAL, 5),
    m_label("Enter integers separated by spaces:")
{
    set_transient_for(parent);
    set_title("Create New Set");
    set_modal(true);

    get_content_area()->append(m_contentBox);
    m_contentBox.set_margin(10);
    m_contentBox.append(m_label);
    m_contentBox.append(m_entry);

    add_button("Cancel", Gtk::ResponseType::CANCEL);
    add_button("Confirm", Gtk::ResponseType::OK);
}
