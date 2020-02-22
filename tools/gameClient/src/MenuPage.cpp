#include <string>
#include "MenuPage.h"

MenuPage::MenuPage( const std::string &menu_name,
                    const MenuPage::NameList &field_names,
                    const MenuPage::NameList &item_names,
                    const MenuPage::FunctionList &item_results )
    : menu_name( menu_name ), 
      field_names( field_names ), 
      item_names( item_names ), 
      item_results( item_results ) { }

std::vector<const char *> MenuPage::get_field_names() {
    return field_names;
}

MenuPage::FieldList *MenuPage::get_field_list() {
    return &field_list;
}

FORM* MenuPage::get_form() {
    return form;
}

void MenuPage::set_form( FORM* form ) {
    this->form = form;
}

void MenuPage::add_field( FIELD *field ) {
    field_list.push_back( field );
}

bool MenuPage::has_form() {
    return !field_names.empty();
}

std::vector<const char *> MenuPage::get_item_names() {
    return item_names;
}

MENU *MenuPage::get_menu() {
    return menu;
}

void MenuPage::set_menu( MENU *menu ) {
    this->menu = menu;
}

void MenuPage::add_item( ITEM *item ) {
    item_list.push_back( item );
}

MenuPage::ItemList *MenuPage::get_item_list() {
    return &item_list;
}

int MenuPage::get_selected_option() {
    return selected_option;
}

int MenuPage::change_selected_option_on_input() {
    selected_option = getch();
    return selected_option;
}

const MenuPage::FunctionList MenuPage::get_item_results() {
    return item_results;
}

std::string MenuPage::get_menu_name() {
    return menu_name;
}

void MenuPage::cleanup() {
    unpost_form( form );
    for ( int i = 0; i < item_names.size(); i++ ) {
        free_item( item_list[i] );
    }
    free_menu( menu );

    unpost_menu( menu );
    for ( int i = 0; i < field_names.size(); i++ ) {
        free_field( field_list[i] );
    }
    free_form( form );
}
