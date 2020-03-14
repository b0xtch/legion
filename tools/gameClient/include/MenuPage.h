#ifndef MENU_PAGE_H
#define MENU_PAGE_H

#include <vector>
#include <functional>
#include <string_view>
#include <map>
#include <menu.h>
#include <form.h>
#include <memory>

struct MenuPageInfo {
    using MenuName = std::string_view;
    using ItemName = const char *;
    using NameList = std::vector<ItemName>;
    using ItemFunction = std::function<void()>;
    using FunctionList = std::vector<ItemFunction>;

    MenuPageInfo( 
                    const MenuName &menuName,
                    const NameList &fieldNames,
                    const NameList &itemNames,
                    const FunctionList &itemResults )
    : menuName( menuName ), 
      fieldNames( fieldNames ), 
      itemNames( itemNames ), 
      itemResults( itemResults ) { }

    MenuName menuName;
    NameList fieldNames;
    NameList itemNames;
    FunctionList itemResults;
};

class MenuPage {

public:

    using PageMap = std::map<MenuPageInfo::MenuName, MenuPage *>;
    using ItemList = std::vector<ITEM *>;
    using FieldList = std::vector<FIELD *>;
    
    MenuPage( const std::shared_ptr<MenuPageInfo> &menuPageInfo )
        : menuName( menuPageInfo->menuName ),
        itemResults( menuPageInfo->itemResults ) { }

    void cleanup();

    int changeSelectedOptionOnInput();

    FieldList* getFieldList();
    void addField( FIELD *field );
    FORM* getForm();
    void setForm( FORM *form );
    bool hasForm();

    std::vector<MenuPageInfo::ItemName> getItemNames();
    const MenuPageInfo::FunctionList getItemResults();
    void addItem( ITEM *item );
    MENU* getMenu();
    void setMenu( MENU *menu );
    ItemList* getItemList();

    MenuPageInfo::MenuName getMenuName();

private:

    MenuPage();

    MenuPageInfo::MenuName menuName;

    // Menu components
    FORM *form;
    FieldList fieldList;

    MENU *menu;
    ItemList itemList;
    const MenuPageInfo::FunctionList itemResults;
};

#endif