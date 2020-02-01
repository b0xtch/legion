#ifndef SETUPMENU_H
#define SETUPMENU_H

#include <vector>
#include <string>
#include <map>
#include <menu.h>
#include <form.h>

class MenuPage {
public:
    
    MenuPage(std::string menu_name,
             std::vector<const char *> fields, 
             std::vector<const char *> options, 
             std::vector<void (*)(std::map<std::string, MenuPage *>, 
                                  MenuPage **current_page)> option_results);

    void cleanup();

    std::vector<const char *> get_fields();
    FORM* get_form();
    bool has_form();

    std::vector<const char *> get_options();
    std::vector<void (*)(std::map<std::string, MenuPage *>, 
                         MenuPage **current_page)> get_option_results();
    MENU* get_menu();
    int get_current_option();
    int change_current_option_on_input();
    int get_options_size();

    static WINDOW *main_window;
    static WINDOW *form_window;
    static WINDOW *menu_window;

private:
    MenuPage();

    std::string menu_name;

    // Menu components
    std::vector<void (*)(std::map<std::string, MenuPage *>, 
                         MenuPage **current_page)> option_results;
    std::vector<const char *> options;

    std::vector<const char *> fields;
    
    FORM *form;
    FIELD **field_list;
    FIELD *current_form;
    int current_field;
    int field_size;

    MENU *menu;
    ITEM **item_list;
    ITEM *current_item;
    int current_option;
    int options_size;
};

#endif