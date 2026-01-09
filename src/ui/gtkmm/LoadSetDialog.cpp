#include "ui/gtkmm/LoadSetDialog.hpp"

LoadSetDialog::LoadSetDialog(Gtk::Window& parent, std::shared_ptr<WrapCore> core)
    :   core_(core),
        m_removeBtn("Delete")
{
    // ColumnView layout setup
    setupColumns();
    m_setsWindow.set_child(m_dbView);
    m_setsWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    m_setsWindow.set_expand();
    // Grid layout setup
    setupGrid();
    // Main window setup
    set_transient_for(parent);
    set_title("Load Set");
    set_modal(true);
    set_default_size(400, 400);

    get_content_area()->append(m_mainGrid);
    add_button("Cancel", Gtk::ResponseType::CANCEL);
    add_button("Confirm", Gtk::ResponseType::OK);
    m_removeBtn.set_halign(Gtk::Align::START);
    // Signals configurations
    m_removeBtn.signal_clicked().connect(sigc::mem_fun(*this, &LoadSetDialog::on_remove_clicked));
}
std::string LoadSetDialog::getSelectedValue()
{
    std::string selectedSet;
    auto selectedItem = m_selection->get_selected_item();
    if (selectedItem)
    {
        auto stringObj = std::dynamic_pointer_cast<Gtk::StringObject>(selectedItem);
        size_t id = std::stoul(stringObj->get_string());
        if (core_->loadSet(id))
        {
            selectedSet = core_->getSetString();
        }
    }
    return selectedSet;
}

//  Helper functions
void LoadSetDialog::setupColumns()
{
// --- ID Column ---
    auto idColumn = Gtk::ColumnViewColumn::create(s_idColumnName_, createIdColumn());
    m_dbView.append_column(idColumn);
// --- Values Column ---
    auto valueColumn = Gtk::ColumnViewColumn::create(s_valueColumnName_, createValueColumn());
    valueColumn->set_expand();
    m_dbView.append_column(valueColumn);
// --- Values StringList
    m_setsIdList = Gtk::StringList::create({});
    refreshDbList();
    m_selection = Gtk::SingleSelection::create(m_setsIdList);
    m_dbView.set_model(m_selection);
}
void LoadSetDialog::setupGrid()
{
    m_mainGrid.set_margin(15);
    m_mainGrid.set_row_spacing(10);
    m_mainGrid.set_column_spacing(10);
    m_mainGrid.attach(m_removeBtn, 0, 0, 1, 1);
    m_mainGrid.attach(m_setsWindow, 0, 1, 3, 1);
}
void LoadSetDialog::refreshDbList()
{
    auto dbSetIds = core_->getIdList();
    bool isSensitive = dbSetIds.size() > 0;
    m_setsIdList->splice(0, m_setsIdList->get_n_items(), {});
    for (auto id : dbSetIds)
    {
        m_setsIdList->append(std::to_string(id));
    }
    m_removeBtn.set_sensitive(isSensitive);
}

// ColumnView factories
Glib::RefPtr<Gtk::SignalListItemFactory> LoadSetDialog::createIdColumn()
{
    auto factory = Gtk::SignalListItemFactory::create();
    factory->signal_setup().connect(
        [](const Glib::RefPtr<Gtk::ListItem>& list_item)
        {
            list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::START));
        }
    );
    factory->signal_bind().connect(
        [](const Glib::RefPtr<Gtk::ListItem>& list_item)
        {
            auto item = std::dynamic_pointer_cast<Gtk::StringObject>(list_item->get_item());
            auto label = dynamic_cast<Gtk::Label*>(list_item->get_child());
            if (item && label) label->set_text(item->get_string());
        }
    );
    return factory;
}
Glib::RefPtr<Gtk::SignalListItemFactory> LoadSetDialog::createValueColumn()
{
    auto factory = Gtk::SignalListItemFactory::create();
    factory->signal_setup().connect(
        [](const Glib::RefPtr<Gtk::ListItem>& list_item)
        {
            list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::START));
        }
    );
    factory->signal_bind().connect(
        [this](const Glib::RefPtr<Gtk::ListItem>& list_item)
        {
            auto item = std::dynamic_pointer_cast<Gtk::StringObject>(list_item->get_item());
            auto label = dynamic_cast<Gtk::Label*>(list_item->get_child());
            if (item && label)
            {
                size_t id = std::stoul(item->get_string());
                label->set_text(core_->getSetString(id)); 
            }
        }
    );
    return factory;
}
// Signals
void LoadSetDialog::on_remove_clicked()
{
    auto m_confirmDialog = Gtk::make_managed<Gtk::MessageDialog>(*this, "Confirm", false, Gtk::MessageType::WARNING, Gtk::ButtonsType::YES_NO, true);
    m_confirmDialog->set_message("Confirm delete operation");
    m_confirmDialog->set_secondary_text("Are you sure you want delete this item?");
    m_confirmDialog->signal_response().connect(
        [this, m_confirmDialog](int response_id)
        {
            if (response_id == Gtk::ResponseType::YES)
            {
                auto selectedItem = m_selection->get_selected_item();
                if (!selectedItem) return;

                auto stringObj = std::dynamic_pointer_cast<Gtk::StringObject>(selectedItem);
                size_t id = std::stoul(stringObj->get_string());
                if (core_->removeSet(id))
                {
                    refreshDbList(); 
                }
            }
            m_confirmDialog->hide();
        }
    );
    m_confirmDialog->show();
}
