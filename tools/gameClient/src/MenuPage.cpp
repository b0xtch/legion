#include "MenuPage.h"

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

bool MenuPage::hasForm() const {
    return !fieldList.empty();
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

bool MenuPage::hasMenu() const {
    return !itemList.empty();
}

MenuPage::ItemList *MenuPage::getItemList() {
    return &itemList;
}

const MenuPageInfo::FunctionList MenuPage::getItemResults() const {
    return itemResults;
}

MenuPageInfo::MenuName MenuPage::getMenuName() const {
    return menuName;
}

void MenuPage::cleanup() {
    unpost_form( form );
    for ( int i = 0; i < itemList.size(); i++ ) {
        free_item( itemList[i] );
    }
    free_menu( menu );

    unpost_menu( menu );
    for ( int i = 0; i < fieldList.size(); i++ ) {
        free_field( fieldList[i] );
    }
    free_form( form );
}
