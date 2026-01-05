#include "./ui/gtkmm/SetWindow.hpp"

SetWindow::SetWindow(std::shared_ptr<WrapCore> core) 
    :   core_(core),
        m_mainBox(Gtk::Orientation::VERTICAL, 10),
        m_lblStatus("Current Set: {}"),
        m_entryInput(),
        m_buttonBox(Gtk::Orientation::HORIZONTAL, 5),

        m_btnAdd("Add"),
        m_btnSave("Save"),
        m_btnLoad("Load by ID")
{
    set_title("Integer Set Lab - Lab 1");
    set_default_size(400, 200);

    set_child(m_mainBox);
    m_mainBox.set_margin(15);

    m_mainBox.append(m_lblStatus);
    m_mainBox.append(m_entryInput);
    m_mainBox.append(m_buttonBox);

    m_buttonBox.append(m_btnAdd);
    m_buttonBox.append(m_btnSave);
    m_buttonBox.append(m_btnLoad);
}
