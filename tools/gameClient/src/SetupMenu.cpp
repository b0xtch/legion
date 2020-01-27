#include <iostream>
#include <ncurses.h>
#include <menu.h>

#include "MenuPage.h"

// Basic menu structure referenced from: 
//    http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/menus.html

void join_server(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page);
void create_server(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page);
void move_back_a_page(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page);
void exit_program(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page);

int main() {

    // Initialize menu pages
    std::map<std::string, MenuPage *> menu_pages;

    // Main menu
    std::vector<const char *> main_menu_options;
    main_menu_options.push_back("Join server");
    main_menu_options.push_back("Create server");
    main_menu_options.push_back("Exit");
    std::vector<void (*)(std::map<std::string, MenuPage *>,
                         MenuPage **current_page)> main_menu_option_results;
    main_menu_option_results.push_back(join_server);
    main_menu_option_results.push_back(create_server);
    main_menu_option_results.push_back(exit_program);
    std::string main_menu_name = "Main menu";
    MenuPage *main_menu_page = new MenuPage(main_menu_name,
                                            main_menu_options, 
                                            main_menu_option_results);
    menu_pages[main_menu_name] = main_menu_page;

    // Join server menu
    std::vector<const char *> join_server_options;
    join_server_options.push_back("Back");
    std::vector<void (*)(std::map<std::string, MenuPage *>, 
                         MenuPage **current_page)> join_server_option_results;
    join_server_option_results.push_back(move_back_a_page);
    std::string join_server_name = "Join server";
    MenuPage *join_server_page = new MenuPage(join_server_name,
                                              join_server_options, 
                                              join_server_option_results);
    menu_pages[join_server_name] = join_server_page;

    // Create server menu
    std::vector<const char *> create_server_options;
    create_server_options.push_back("Back");
    std::vector<void (*)(std::map<std::string, MenuPage *>, 
                         MenuPage **current_page)> create_server_option_results;
    create_server_option_results.push_back(move_back_a_page);                     
    std::string create_server_name = "Create server";
    MenuPage *create_server_page = new MenuPage(create_server_name,
                                                create_server_options, 
                                                create_server_option_results);
    menu_pages[create_server_name] = create_server_page;                                            

    MenuPage *current_page = main_menu_page;

    // Initialize ncurses
    initscr();
    noecho();
    cbreak();

    keypad(stdscr, TRUE);

    // Use current menu page
    post_menu(current_page->get_menu());
    refresh();

    int selected_index = 0;

    // Menu loop
    while (current_page->change_current_option_on_input()) {
        switch (current_page->get_current_option()) {
            case KEY_DOWN:
                menu_driver(current_page->get_menu(), REQ_DOWN_ITEM);
                if (selected_index < current_page->get_options_size()) {
                    selected_index++;
                }
                break;

            case KEY_UP:
                menu_driver(current_page->get_menu(), REQ_UP_ITEM);
                if (selected_index > 0) {
                    selected_index--;
                }
                break;

             case '\n':
                // Menu item chosen. Call corresponding function 
                current_page->
                    get_option_results()[selected_index](menu_pages, &current_page);
                selected_index = 0;
                break;
        }
    }

    delete main_menu_page;
    delete join_server_page;
    delete create_server_page;

    endwin();

    return 0;
}

void join_server(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page) {
    
    unpost_menu((*current_page)->get_menu());
    *current_page = menu_pages["Join server"];
    post_menu((*current_page)->get_menu());
    refresh();
}

void create_server(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page) {

    unpost_menu((*current_page)->get_menu());
    *current_page = menu_pages["Create server"];
    post_menu((*current_page)->get_menu());
    refresh();
}

void move_back_a_page(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page) {

    unpost_menu((*current_page)->get_menu());
    *current_page = menu_pages["Main menu"];
    post_menu((*current_page)->get_menu());
    refresh();
    
}

void exit_program(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page) {
    // TODO: Make it exit the menu loop
}