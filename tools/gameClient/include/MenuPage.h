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
    
    MenuPage( const MenuName &menu_name,
              const NameList &field_names, 
              const NameList &item_names, 
              const FunctionList &item_results );

    void cleanup();

    int get_selected_option();
    int change_selected_option_on_input();

    std::vector<ItemName> get_field_names();
    FieldList* get_field_list();
    void add_field( FIELD *field );
    FORM* get_form();
    void set_form( FORM *form );
    bool has_form();

    std::vector<ItemName> get_item_names();
    const FunctionList get_item_results();
    void add_item( ITEM *item );
    MENU* get_menu();
    void set_menu( MENU *menu );
    ItemList* get_item_list();

    MenuName get_menu_name();

private:

    MenuPage();

    MenuName menu_name;
    int selected_option;

    // Menu components
    FORM *form;
    std::vector<ItemName> field_names;
    std::vector<FIELD *> field_list;

    MENU *menu;
    std::vector<ItemName> item_names;
    ItemList item_list;
    const FunctionList item_results;
};

#endif