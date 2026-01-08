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

    m_errorDialog.signal_response().connect(
        [this](int response_id) {
            m_errorDialog.hide();
        }
    );

    m_infoDialog.signal_response().connect(
        [this](int response_id) {
            m_infoDialog.hide();
        }
    );
    // Initialize the list model
    m_StringList = Gtk::StringList::create({});
    refreshIdList();
    auto selection_model = Gtk::SingleSelection::create(m_StringList);
    m_ColumnView.set_model(selection_model);
    //  Create the ID Column
    auto column = Gtk::ColumnViewColumn::create("Available Set IDs", createIdColumn());
    m_ColumnView.append_column(column);
    // Add to layout
    m_ScrolledWindow.set_child(m_ColumnView);
    m_ScrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    m_ScrolledWindow.set_expand();
    m_mainBox.append(m_ScrolledWindow);

    m_btnAdd.signal_clicked().connect(sigc::mem_fun(*this, &SetWindow::on_add_clicked));
    m_btnSave.signal_clicked().connect(sigc::mem_fun(*this, &SetWindow::on_save_clicked));
    m_btnLoad.signal_clicked().connect(sigc::mem_fun(*this, &SetWindow::on_load_clicked));
}


// Signals
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

void SetWindow::on_load_clicked()
{
    std::string text = m_entryInput.get_text();
    if (text.empty()) {
        m_errorDialog.set_message("Input Error");
        m_errorDialog.set_secondary_text("Please enter a valid Set ID in the text field.");
        m_errorDialog.show();
        return;
    }

    try {
        // Convert string to size_t
        size_t id = std::stoul(text);

        if (core_->loadSet(id)) {
            m_lblStatus.set_text(lblStatusString + core_->getSetString());
            m_infoDialog.set_message("Load Success");
            m_infoDialog.set_secondary_text("Set with ID " + std::to_string(id) + " loaded successfully.");
            m_infoDialog.show();
        } else {
            m_errorDialog.set_message("Load Failed");
            m_errorDialog.set_secondary_text("Could not find a set with ID: " + std::to_string(id));
            m_errorDialog.show();
        }
    } catch (const std::exception& e) {
        m_errorDialog.set_message("Invalid ID");
        m_errorDialog.set_secondary_text("Please enter a numeric value. Error: " + std::string(e.what()));
        m_errorDialog.show();
    }
}

// Factory helper functions
Glib::RefPtr<Gtk::SignalListItemFactory> SetWindow::createIdColumn()
{
    auto factory = Gtk::SignalListItemFactory::create();
    // Create a View
    factory->signal_setup().connect
    (
        [](const Glib::RefPtr<Gtk::ListItem>& list_item)
        {
            list_item->set_child(*Gtk::make_managed<Gtk::Label>(""));
        }
    );
    //Bind a Model to View
    factory->signal_bind().connect
    (
        [](const Glib::RefPtr<Gtk::ListItem>& list_item)
        {
            auto item = std::dynamic_pointer_cast<Gtk::StringObject>(list_item->get_item());
            auto label = dynamic_cast<Gtk::Label*>(list_item->get_child());
            if (item && label) label->set_text(item->get_string());
        }
    );
    return factory;
}

void SetWindow::refreshIdList() {
    auto ids = core_->getIdList();
    m_StringList->splice(0, m_StringList->get_n_items(), {});
    for (auto id : ids) {
        m_StringList->append(std::to_string(id));
    }
}
