#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <vector>
#include <functional>
#include <map>
#include <menu.h>
#include <form.h>


class MenuManager {

public:

    MenuManager();

    void initialize_windows();
    void initialize_starting_page();
    void add_page( MenuPage *page );
    void set_current_page( MenuPage *page );
    MenuPage *get_current_page();
    int get_selected_index();
    void switch_page( MenuPage::MenuName &next_page_name );
    void main_menu_driver();
    void cleanup();

private:

    WINDOW *main_window;
    WINDOW *form_window;
    WINDOW *menu_window;

    std::map<MenuPage::MenuName, MenuPage *> menu_pages;
    MenuPage *current_page;
    int selected_index;
    bool is_on_menu;

};

#endif