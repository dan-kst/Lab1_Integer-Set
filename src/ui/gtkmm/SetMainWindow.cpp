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
// ColumnView layout setup
    setupColumns();
    m_setsWindow.set_child(m_setListView);
    m_setsWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    m_setsWindow.set_expand();
// Local operations layout setup
    m_localOperationsBox.append(m_removeBtn);
    m_localOperationsBox.append(m_editBtn);
    m_localOperationsBox.set_halign(Gtk::Align::CENTER);
// Database operations layout setup
    m_dbOperationsBox.append(m_saveBtn);
    m_dbOperationsBox.append(m_loadBtn);
    m_dbOperationsBox.set_halign(Gtk::Align::END);
// Set operations layout setup
    m_setOperationsBox.append(m_unionBtn);
    m_setOperationsBox.append(m_interBtn);
    m_setOperationsBox.append(m_diffBtn);
// Grid layout
    setupGrid();
// Main window setup
    set_title("Integer Set Main Menu");
    set_child(m_mainGrid);
    set_default_size(600, 600);

// Signals configurations
    m_errorDialog.signal_response().connect([this](int) { m_errorDialog.hide(); });
    m_infoDialog.signal_response().connect([this](int) { m_infoDialog.hide(); });
    m_createBtn.signal_clicked().connect(sigc::mem_fun(*this, &SetMainWindow::on_create_clicked));
    m_loadBtn.signal_clicked().connect(sigc::mem_fun(*this, &SetMainWindow::on_load_clicked));
    m_removeBtn.signal_clicked().connect(sigc::mem_fun(*this, &SetMainWindow::on_remove_clicked));
}


// Helper functions
void SetMainWindow::setupColumns()
{
// --- List of values ---
    m_setValueStringList = Gtk::StringList::create({});
    refreshLocalList();
    m_selection = Gtk::SingleSelection::create(m_setValueStringList);
    m_setListView.set_model(m_selection);
// --- Values Column ---
    auto valueColumn = Gtk::ColumnViewColumn::create(s_valueColumnName_, createValueColumn());
    valueColumn->set_expand();
    m_setListView.append_column(valueColumn);
}
void SetMainWindow::setupGrid()
{
    m_mainGrid.set_margin(15);
    m_mainGrid.set_row_spacing(10);
    m_mainGrid.set_column_spacing(10);
    // Row 0: CRUD operations
    m_mainGrid.attach(m_createBtn, 0, 0, 1, 1);
    m_createBtn.set_halign(Gtk::Align::START);
    m_mainGrid.attach(m_localOperationsBox, 1, 0, 2, 1);
    m_mainGrid.attach(m_dbOperationsBox, 3, 0, 1, 1);
    // Row 1: Local list of integer sets
    m_mainGrid.attach(m_setsWindow, 0, 1, 4, 1);
    // Row 2: Math operations
    m_mainGrid.attach(m_setOperationsBox, 0, 2, 3, 1);
    // Row 3: Math operations result
    m_mainGrid.attach(m_resultLabel, 0, 4, 4, 1);
}
void SetMainWindow::refreshLocalList()
{
    bool isSensitive = localSetValues_.size() > 0;
    m_setValueStringList->splice(0, m_setValueStringList->get_n_items(), {});
    for (auto value : localSetValues_)
    {
        m_setValueStringList->append(value);
    }
    m_removeBtn.set_sensitive(isSensitive);
    m_editBtn.set_sensitive(isSensitive);
    m_unionBtn.set_sensitive(isSensitive);
    m_interBtn.set_sensitive(isSensitive);
    m_diffBtn.set_sensitive(isSensitive);
}


// ColumnView factory
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


// Signals
void SetMainWindow::on_create_clicked()
{
    auto createDialog = Gtk::make_managed<CreateSetDialog>(*this);
    
    createDialog->signal_response().connect(
        [this, createDialog](int response_id)
        {
            if (response_id == Gtk::ResponseType::OK)
            {
                std::istringstream iss(createDialog->getInputValue());
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
                std::string selectedSet(loadDialog->getSelectedValue());
                if(!selectedSet.empty())
                {
                    localSetValues_.push_back(core_->getSetString());
                    refreshLocalList();
                    m_infoDialog.set_message("Success!");
                    m_infoDialog.set_secondary_text("Set was load to your list");
                    m_infoDialog.show();
                }
                else
                {
                    m_errorDialog.set_message("Error!");
                    m_errorDialog.set_secondary_text("Failed to load a set!");
                    m_errorDialog.show();
                }
            }
            loadDialog->hide();
        }
    );
    loadDialog->show();
}
void SetMainWindow::on_remove_clicked()
{
    if(localSetValues_.size() > 0)
    {
        auto m_confirmDialog = Gtk::make_managed<Gtk::MessageDialog>(*this, "Confirm", false, Gtk::MessageType::WARNING, Gtk::ButtonsType::YES_NO, true);
        m_confirmDialog->set_message("Confirm delete operation");
        m_confirmDialog->set_secondary_text("Are you sure you want delete this item?");
        m_confirmDialog->signal_response().connect(
            [this, m_confirmDialog](int response_id)
            {
                if (response_id == Gtk::ResponseType::YES)
                {
                    auto selectedItemId = m_selection->get_selected();
                    if (selectedItemId != GTK_INVALID_LIST_POSITION)
                    {
                        size_t count = 0;
                        std::vector<std::string>::iterator it = localSetValues_.begin();
                        while(it != localSetValues_.end())
                        {
                            if (count == selectedItemId)
                            {
                                it = localSetValues_.erase(it);
                            }
                            else
                            {
                                it++;
                            }
                            count++;
                        }
                        refreshLocalList();
                        m_infoDialog.set_message("Success!");
                        m_infoDialog.set_secondary_text("Set was removed to your list");
                        m_infoDialog.show();
                    }
                }
                m_confirmDialog->hide();
            }
        );
        m_confirmDialog->show();
        
    }
}
