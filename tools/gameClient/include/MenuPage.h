#ifndef MENU_PAGE_H
#define MENU_PAGE_H

#include <vector>
#include <functional>
#include <string>
#include <map>
#include <menu.h>
#include <form.h>

class MenuPage {

public:

    using PageMap = std::map<std::string, MenuPage *>;
    using ItemFunction = std::function<void()>;
    using FunctionList = std::vector<ItemFunction>;
    using NameList = std::vector<const char *>;
    using ItemList = std::vector<ITEM *>;
    using FieldList = std::vector<FIELD *>;
    
    MenuPage( const std::string &menu_name,
              const NameList &field_names, 
              const NameList &item_names, 
              const FunctionList &item_results );

    void cleanup();

    int get_selected_option();
    int change_selected_option_on_input();

    std::vector<const char *> get_field_names();
    FieldList* get_field_list();
    void add_field( FIELD *field );
    FORM* get_form();
    void set_form( FORM *form );
    bool has_form();

    std::vector<const char *> get_item_names();
    const FunctionList get_item_results();
    void add_item( ITEM *item );
    MENU* get_menu();
    void set_menu( MENU *menu );
    ItemList* get_item_list();

    std::string get_menu_name();

private:

    MenuPage();

    std::string menu_name;
    int selected_option;

    // Menu components
    FORM *form;
    std::vector<const char *> field_names;
    std::vector<FIELD *> field_list;

    MENU *menu;
    std::vector<const char *> item_names;
    ItemList item_list;
    const FunctionList item_results;
};

#endif