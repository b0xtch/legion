#include "MenuPage.h"

MenuPage::MenuPage( const MenuPage::MenuName &menuName,
                    const MenuPage::NameList &fieldNames,
                    const MenuPage::NameList &itemNames,
                    const MenuPage::FunctionList &itemResults )
    : menuName( menuName ), 
      fieldNames( fieldNames ), 
      itemNames( itemNames ), 
      itemResults( itemResults ) { }

std::vector<MenuPage::ItemName> MenuPage::getFieldNames() {
    return fieldNames;
}

MenuPage::FieldList *MenuPage::getFieldList() {
    return &fieldList;
}

FORM* MenuPage::getForm() {
    return form;
}

void MenuPage::setForm( FORM* form ) {
    this->form = form;
}

void MenuPage::addField( FIELD *field ) {
    fieldList.push_back( field );
}

bool MenuPage::hasForm() {
    return !fieldNames.empty();
}

std::vector<MenuPage::ItemName> MenuPage::getItemNames() {
    return itemNames;
}

MENU *MenuPage::getMenu() {
    return menu;
}

void MenuPage::setMenu( MENU *menu ) {
    this->menu = menu;
}

void MenuPage::addItem( ITEM *item ) {
    itemList.push_back( item );
}

MenuPage::ItemList *MenuPage::getItemList() {
    return &itemList;
}

int MenuPage::getSelectedOption() {
    return selectedOption;
}

int MenuPage::changeSelectedOptionOnInput() {
    selectedOption = getch();
    return selectedOption;
}

const MenuPage::FunctionList MenuPage::getItemResults() {
    return itemResults;
}

MenuPage::MenuName MenuPage::getMenuName() {
    return menuName;
}

void MenuPage::cleanup() {
    unpost_form( form );
    for ( int i = 0; i < itemNames.size(); i++ ) {
        free_item( itemList[i] );
    }
    free_menu( menu );

    unpost_menu( menu );
    for ( int i = 0; i < fieldNames.size(); i++ ) {
        free_field( fieldList[i] );
    }
    free_form( form );
}
