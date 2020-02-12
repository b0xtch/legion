#ifndef SETUPMENU_H
#define SETUPMENU_H

#include <vector>
#include <string>
#include <map>
#include <menu.h>
#include <form.h>

class MenuPage {

public:

    using PageMap = std::map<std::string, MenuPage *>;
    using FunctionList = std::vector<void (*)()>;
    using NameList = const std::vector<const char *>;
    
    MenuPage( const std::string &menu_name,
              NameList &field_names, 
              NameList &item_names, 
              const FunctionList &item_results );

    void cleanup();
    
    static WINDOW *main_window;
    static WINDOW *form_window;
    static WINDOW *menu_window;

    int get_selected_option();
    int change_selected_option_on_input();

    std::vector<const char *> get_field_names();
    std::vector<FIELD *> get_field_list();
    FORM* get_form();
    bool has_form();

    std::vector<const char *> get_item_names();
    const FunctionList get_item_results();
    MENU* get_menu();

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
    std::vector<ITEM *> item_list;
    const FunctionList item_results;
};



class MenuManager {

public:

    static void initialize_menu_manager();
    static void initialize_windows();
    static void add_menu_page( std::string &page_name, MenuPage *page );
    static std::map<std::string, MenuPage *> get_menu_pages();
    static void set_current_page( MenuPage *page );
    static MenuPage *get_current_page();
    static void switch_page( std::string &next_page_name );
    static void main_loop();
    static void cleanup();

private:
    static std::map<std::string, MenuPage *> menu_pages;
    static MenuPage *current_page;
    static int selected_index;
    static bool is_on_menu;

};

#endif
