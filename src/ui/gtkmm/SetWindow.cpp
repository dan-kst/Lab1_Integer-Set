#include "./ui/gtkmm/SetWindow.hpp"

SetWindow::SetWindow(std::shared_ptr<WrapCore> core)
    :   core_(core),
        m_mainBox(Gtk::Orientation::VERTICAL, 10),
        m_lblStatus(lblStatusString),
        m_entryInput(),
        m_buttonBox(Gtk::Orientation::HORIZONTAL, 5),

        m_errorDialog(  *this, 
                        "Error",
                        false, 
                        Gtk::MessageType::ERROR,
                        Gtk::ButtonsType::OK,
                        true
                    ),
        m_infoDialog(   *this, 
                        "Info",
                        false, 
                        Gtk::MessageType::INFO,
                        Gtk::ButtonsType::OK,
                        true
                    ),

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

    m_errorDialog.signal_response().connect([this](int response_id) {
        m_errorDialog.hide();
    });

    m_infoDialog.signal_response().connect([this](int response_id) {
        m_infoDialog.hide();
    });
    m_btnAdd.signal_clicked().connect(sigc::mem_fun(*this, &SetWindow::on_add_clicked));
    m_btnSave.signal_clicked().connect(sigc::mem_fun(*this, &SetWindow::on_save_clicked));
}

void SetWindow::on_add_clicked()
{
    std::istringstream iss(m_entryInput.get_text());
    core_->createSet(iss);
    m_entryInput.set_text("");
    m_lblStatus.set_text(lblStatusString + core_->getSetString());
}

void SetWindow::on_save_clicked() 
{
    try
    {
        size_t id = core_->saveSet();
        if(id)
        {
            m_infoDialog.set_message("Save success!");
            m_infoDialog.set_secondary_text("New set saved successfully with ID: " + std::to_string(id));
            m_infoDialog.show();
        }
    }
    catch (const std::exception& e)
    {
        m_errorDialog.set_message("Failed to save!");
        m_errorDialog.set_secondary_text(e.what());
        m_errorDialog.show();
    }
}
