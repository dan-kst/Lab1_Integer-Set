#include "./ui/gtkmm/SetMainWindow.hpp"

SetMainWindow::SetMainWindow(std::shared_ptr<WrapCore> core)
    :   core_(core),
        m_localOperationsBox(Gtk::Orientation::HORIZONTAL, 16),
        m_dbOperationsBox(Gtk::Orientation::HORIZONTAL, 16),
        m_setOperationsBox(Gtk::Orientation::HORIZONTAL, 8),

        m_createBtn("Create"),
        m_removeBtn("Remove"),
        m_editBtn("Edit"),
        m_saveBtn("Save"),
        m_loadBtn("Load"),
        m_unionBtn("Union"),
        m_interBtn("Intersect"),
        m_diffBtn("Difference"),

        m_resultLabel(s_resultLabelValue_),

        m_errorDialog(*this, "Error", false, Gtk::MessageType::ERROR, Gtk::ButtonsType::OK, true),
        m_infoDialog(*this, "Info", false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK, true)
{
    set_title("Integer Set Main Menu");
    set_child(m_mainGrid);

    // Minimal sizing: make the window only as large as its children
    set_resizable(false); 
    m_mainGrid.set_margin(15);
    m_mainGrid.set_row_spacing(10);
    m_mainGrid.set_column_spacing(10);

    // Local operations layout setup
    m_localOperationsBox.append(m_removeBtn);
    m_localOperationsBox.append(m_editBtn);

    // Database operations layout setup
    m_dbOperationsBox.append(m_saveBtn);
    m_dbOperationsBox.append(m_loadBtn);

    // Set operations layout setup
    m_setOperationsBox.append(m_unionBtn);
    m_setOperationsBox.append(m_interBtn);
    m_setOperationsBox.append(m_diffBtn);

    // Initialize the list model
    m_setValueStringList = Gtk::StringList::create({});
    refreshLocalList();
    auto selection_model = Gtk::SingleSelection::create(m_setValueStringList);
    m_setListView.set_model(selection_model);
    // Create the Value Column
    auto column = Gtk::ColumnViewColumn::create(s_valueColumnName_, createValueColumn());
    m_setListView.append_column(column);
    // Add to layout
    m_setsWindow.set_child(m_setListView);
    m_setsWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    m_setsWindow.set_expand();

    // Row 0: CRUD operations
    m_mainGrid.attach(m_createBtn, 0, 0, 1, 1);
    m_mainGrid.attach(m_localOperationsBox, 1, 0, 1, 1);
    m_mainGrid.attach(m_dbOperationsBox, 2, 0, 1, 1);

    // Row 1: Local list of integer sets
    m_mainGrid.attach(m_setsWindow, 0, 1, 4, 1);

    // Row 2: Math operations
    m_mainGrid.attach(m_setOperationsBox, 0, 2, 3, 1);
    
    // Row 3: Math operations result
    m_mainGrid.attach(m_resultLabel, 0, 4, 4, 1);

    // Configure buttons click signals
    m_errorDialog.signal_response().connect([this](int) { m_errorDialog.hide(); });
    m_infoDialog.signal_response().connect([this](int) { m_infoDialog.hide(); });
    m_createBtn.signal_clicked().connect(sigc::mem_fun(*this, &SetMainWindow::on_create_clicked));
    m_loadBtn.signal_clicked().connect(sigc::mem_fun(*this, &SetMainWindow::on_load_clicked));
}

// ColumnView helper functions
Glib::RefPtr<Gtk::SignalListItemFactory> SetMainWindow::createValueColumn()
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
void SetMainWindow::refreshLocalList() {
    m_setValueStringList->splice(0, m_setValueStringList->get_n_items(), {});
    for (auto value : localSetValues_) {
        m_setValueStringList->append(value);
    }
}


// Signals
void SetMainWindow::on_create_clicked()
{
    auto createDialog = Gtk::make_managed<CreateSetDialog>(*this);
    
    createDialog->signal_response().connect(
        [this, createDialog](int response_id)
        {
            if (response_id == Gtk::ResponseType::OK)
            {
                std::istringstream iss(createDialog->get_data());
                if(core_->createSet(iss))
                {
                    localSetValues_.push_back(core_->getSetString());
                    refreshLocalList();
                    m_infoDialog.set_message("Success!");
                    m_infoDialog.set_secondary_text("Set was added to your list");
                    m_infoDialog.show();
                }
                else
                {
                    m_errorDialog.set_message("Error!");
                    m_errorDialog.set_secondary_text("Failed to create a set!");
                    m_errorDialog.show();
                }
            }
            createDialog->hide();
        }
    );
    createDialog->show();
}
void SetMainWindow::on_load_clicked()
{
    auto loadDialog = Gtk::make_managed<LoadSetDialog>(*this, core_);
    
    loadDialog->signal_response().connect(
        [this, loadDialog](int response_id)
        {
            if (response_id == Gtk::ResponseType::OK)
            {
                m_infoDialog.set_message("Success!");
                m_infoDialog.set_secondary_text("Set was loaded to your list");
                m_infoDialog.show();
            }
            loadDialog->hide();
        }
    );
    loadDialog->show();
}
