#ifndef MENU_PAGE_H
#define MENU_PAGE_H

#include <vector>
#include <functional>
#include <string_view>
#include <map>
#include <menu.h>
#include <form.h>

class MenuPage {

public:

    using MenuName = std::string_view;
    using PageMap = std::map<MenuName, MenuPage *>;
    using ItemFunction = std::function<void()>;
    using FunctionList = std::vector<ItemFunction>;
    using ItemName = const char *;
    using NameList = std::vector<ItemName>;
    using ItemList = std::vector<ITEM *>;
    using FieldList = std::vector<FIELD *>;
    
    MenuPage( const MenuName &menuName,
              const NameList &fieldNames, 
              const NameList &itemNames, 
              const FunctionList &itemResults );

    void cleanup();

    int changeSelectedOptionOnInput();

    std::vector<ItemName> getFieldNames();
    FieldList* getFieldList();
    void addField( FIELD *field );
    FORM* getForm();
    void setForm( FORM *form );
    bool hasForm();

    std::vector<ItemName> getItemNames();
    const FunctionList getItemResults();
    void addItem( ITEM *item );
    MENU* getMenu();
    void setMenu( MENU *menu );
    ItemList* getItemList();

    MenuName getMenuName();

private:

    MenuPage();

    MenuName menuName;

    // Menu components
    FORM *form;
    std::vector<ItemName> fieldNames;
    std::vector<FIELD *> fieldList;

    MENU *menu;
    std::vector<ItemName> itemNames;
    ItemList itemList;
    const FunctionList itemResults;
};

#endif