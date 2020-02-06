#include <iostream>
#include <ncurses.h>
#include <menu.h>
#include <form.h>

#include "../include/MenuPage.h"

// Basic menu structure referenced from: 
//    http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/menus.html

// Basic menu structure referenced from: 
//    http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/forms.html

// Idea on combining menus and forms on one page referenced from:
//    https://alan-mushi.github.io/2015/05/26/simple-ncurses-popup-in-C.html

void join_server(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page);
void create_server(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page);
void move_back_a_page(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page);
void exit_program(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page);

void switch_page(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page,
                 std::string next_page_name);

int main() {

    // Initialize ncurses
    initscr();
    noecho();
    cbreak();

    keypad(stdscr, TRUE);



    // Initialize windows
    const int main_window_rows = 24;
    const int main_window_cols = 80;
    const int main_window_top  = (LINES - 25) / 2;
    const int main_window_left = (COLS - 81) / 2;

    const int form_window_rows   = main_window_rows - 10;
    const int form_window_cols   = main_window_cols - 2;
    const int form_window_top    = 1;
    const int form_window_left   = 1;
    
    const int menu_window_rows   = main_window_rows - form_window_rows - 2;
    const int menu_window_cols   = main_window_cols - 2;
    const int menu_window_top    = form_window_top + form_window_rows;
    const int menu_window_left   = 1;

    MenuPage::main_window = newwin(main_window_rows, main_window_cols, 
                                   main_window_left, main_window_top);
    box(MenuPage::main_window, 0, 0);
    MenuPage::form_window = derwin(MenuPage::main_window, 
                                   form_window_rows, form_window_cols, 
                                   form_window_top, form_window_left);
	box(MenuPage::form_window, 0, 0);
    MenuPage::menu_window = derwin(MenuPage::main_window, 
                                   menu_window_rows, menu_window_cols, 
                                   menu_window_top, menu_window_left);
	box(MenuPage::menu_window, 0, 0);



    // Initialize menu pages
    std::map<std::string, MenuPage *> menu_pages;

    // Main menu
    std::vector<const char *> main_menu_fields;
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
                                            main_menu_fields, 
                                            main_menu_options, 
                                            main_menu_option_results);
    menu_pages[main_menu_name] = main_menu_page;

    // Join server menu
    std::vector<const char *> join_server_fields;
    join_server_fields.push_back("Server code:");
    join_server_fields.push_back("");
    std::vector<const char *> join_server_options;
    join_server_options.push_back("Join");
    join_server_options.push_back("Back");
    std::vector<void (*)(std::map<std::string, MenuPage *>, 
                         MenuPage **current_page)> join_server_option_results;
    join_server_option_results.push_back(move_back_a_page);
    join_server_option_results.push_back(move_back_a_page);
    std::string join_server_name = "Join server";
    MenuPage *join_server_page = new MenuPage(join_server_name,
                                              join_server_fields,
                                              join_server_options, 
                                              join_server_option_results);
    menu_pages[join_server_name] = join_server_page;

    // Create server menu
    std::vector<const char *> create_server_fields;
    create_server_fields.push_back("Game JSON:");
    create_server_fields.push_back("");
    std::vector<const char *> create_server_options;
    create_server_options.push_back("Create game");
    create_server_options.push_back("Back");
    std::vector<void (*)(std::map<std::string, MenuPage *>, 
                         MenuPage **current_page)> create_server_option_results;
    create_server_option_results.push_back(move_back_a_page);       
    create_server_option_results.push_back(move_back_a_page);                
    std::string create_server_name = "Create server";
    MenuPage *create_server_page = new MenuPage(create_server_name,
                                                create_server_fields, 
                                                create_server_options, 
                                                create_server_option_results);
    menu_pages[create_server_name] = create_server_page;                                            

    MenuPage *current_page = main_menu_page;



    // Use current menu page
    post_form(current_page->get_form());
    post_menu(current_page->get_menu());
    refresh();
    wrefresh(MenuPage::main_window);
    wrefresh(MenuPage::menu_window);
    wrefresh(MenuPage::form_window);

    int selected_index = 0;

    bool is_on_menu = true;



    // Menu loop
    while (current_page->change_current_option_on_input()) {
        switch (current_page->get_current_option()) {
            case KEY_DOWN:
                if (is_on_menu) {
                    menu_driver(current_page->get_menu(), REQ_DOWN_ITEM);
                    if (selected_index < current_page->get_options_size()) {
                        selected_index++;
                    }
                    wrefresh(MenuPage::menu_window);
                } else {
                    // Switch down to menu
                    selected_index = 0;
                    is_on_menu = true;
                    menu_driver(current_page->get_menu(), REQ_FIRST_ITEM);
                    set_menu_fore(current_page->get_menu(), A_REVERSE);
                    wrefresh(MenuPage::form_window);
                }
                break;

            case KEY_UP:
                if (is_on_menu) {
                    if (selected_index > 0) {
                        selected_index--;
                        menu_driver(current_page->get_menu(), REQ_UP_ITEM);
                    } else if (current_page->has_form()) {
                        // Switch up to form
                        is_on_menu = false;
                        set_menu_fore(current_page->get_menu(), A_NORMAL);
                        form_driver(current_page->get_form(), REQ_PREV_FIELD);
                    }
                    wrefresh(MenuPage::menu_window);
                } else {
                    form_driver(current_page->get_form(), REQ_PREV_FIELD);
                    wrefresh(MenuPage::form_window);
                }
                break;

            case KEY_BACKSPACE:
                if (!is_on_menu) {
                    form_driver(current_page->get_form(), REQ_DEL_PREV);
                    wrefresh(MenuPage::form_window);
                }
                break;

            case KEY_RIGHT:
                if (!is_on_menu) {
                    form_driver(current_page->get_form(), REQ_NEXT_CHAR);
                    wrefresh(MenuPage::form_window);
                }
                break;
            
            case KEY_LEFT:
                if (!is_on_menu) {
                    form_driver(current_page->get_form(), REQ_PREV_CHAR);
                    wrefresh(MenuPage::form_window);
                }
                break;

             case '\n':
                // Menu item chosen. Call corresponding function 
                if (is_on_menu) {
                    menu_driver(current_page->get_menu(), REQ_FIRST_ITEM);
                    current_page->
                        get_option_results()[selected_index](menu_pages, &current_page);
                    selected_index = 0;
                }
                break;

            default:
                if (!is_on_menu) {
                    form_driver(current_page->get_form(), current_page->get_current_option());
                    wrefresh(MenuPage::form_window);
                }

                break;
        }
        wrefresh(MenuPage::main_window);
    }

    main_menu_page->cleanup();
    join_server_page->cleanup();
    create_server_page->cleanup();

    endwin();

    return 0;
}

void join_server(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page) {
    switch_page(menu_pages, current_page, "Join server");
}

void create_server(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page) {
    switch_page(menu_pages, current_page, "Create server");
}

void move_back_a_page(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page) {
    switch_page(menu_pages, current_page, "Main menu");
    
}

void exit_program(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page) {
    
}

void switch_page(std::map<std::string, MenuPage *> menu_pages, MenuPage **current_page, 
                 std::string next_page_name) {
    unpost_form((*current_page)->get_form());
    unpost_menu((*current_page)->get_menu());
    *current_page = menu_pages[next_page_name];
    post_form((*current_page)->get_form());
    post_menu((*current_page)->get_menu());
    refresh();
    wrefresh(MenuPage::main_window);
    wrefresh(MenuPage::menu_window);
    wrefresh(MenuPage::form_window);
}