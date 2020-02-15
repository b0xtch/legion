#include <string>
#include "MenuPage.h"

WINDOW * MenuPage::main_window = nullptr;
WINDOW * MenuPage::form_window = nullptr;
WINDOW * MenuPage::menu_window = nullptr;

MenuPage::MenuPage( const std::string &menu_name,
                    const MenuPage::NameList &field_names,
                    const MenuPage::NameList &item_names,
                    const MenuPage::FunctionList &item_results )
    : menu_name( menu_name ), 
      field_names( field_names ), 
      item_names( item_names ), 
      item_results( item_results ) {

    const int margin_left = 20;
    const int margin_top = 2;

    // Initialize forms
    if ( has_form() ) {
        const int margin_left_field = 5;
        int field_index = 0;

        for ( int i = 0; i < field_names.size() * 2; i++ ) {
            bool is_field_name = i % 2 == 0;

            if ( is_field_name ) {
                // Add field name to form
                field_list.push_back( new_field( 1, 20, margin_top + field_index * 2, 
                                               margin_left_field, 0, 0 ) );
                field_opts_off( field_list[i], O_ACTIVE );
                field_opts_off( field_list[i], O_EDIT );
                set_field_buffer( field_list[i], 0, field_names[field_index] );
                field_index++;
            } 
            else {
                // Add field entry to form
                field_list.push_back( new_field( 1, 20, margin_top + i - 1, 
                                          margin_left, 0, 0 ) );
                field_opts_on( field_list[i], O_ACTIVE );
                field_opts_on( field_list[i], O_EDIT );
                set_field_back( field_list[i], A_UNDERLINE ); 
                set_field_buffer( field_list[i], 0, "" );           
            }
        }

        form = new_form( field_list.data() );
        set_form_win( form, form_window );
        WINDOW *form_inner = derwin( form_window, form->rows + 1, form->cols + 1, 1, 1 );
        set_form_sub( form, form_inner );
    } 

    // Initialize menu
    for ( int i = 0; i < item_names.size(); i++ ) {
        item_list.push_back( new_item( item_names[i], item_names[i] ) );
    }

    menu = new_menu( item_list.data() );

    int max_menu_item_length = 0;
    for ( std::string item_name : item_names ) {
        if ( item_name.length() > max_menu_item_length ) {
            max_menu_item_length = item_name.length() + 1;
        }
    }
	set_menu_win( menu, menu_window );
	WINDOW *menu_inner = derwin( menu_window, item_names.size(), max_menu_item_length, 
                                margin_top, margin_left );
	set_menu_sub( menu, menu_inner );
}

std::vector<const char *> MenuPage::get_field_names() {
    return field_names;
}

std::vector<FIELD *> MenuPage::get_field_list() {
    return field_list;
}

FORM* MenuPage::get_form() {
    return form;
}

bool MenuPage::has_form() {
    return !field_names.empty();
}

std::vector<const char *> MenuPage::get_item_names() {
    return item_names;
}

MENU* MenuPage::get_menu() {
    return menu;
}

int MenuPage::get_selected_option() {
    return selected_option;
}

int MenuPage::change_selected_option_on_input() {
    selected_option = getch();
    return selected_option;
}

const MenuPage::FunctionList MenuPage::get_item_results() {
    return item_results;
}

void MenuPage::cleanup() {
    unpost_form( form );
    for ( int i = 0; i < item_names.size(); i++ ) {
        free_item( item_list[i] );
    }
    free_menu( menu );

    unpost_menu( menu );
    for ( int i = 0; i < field_names.size(); i++ ) {
        free_field( field_list[i] );
    }
    free_form( form );

	delwin( form_window );
	delwin( menu_window );
	delwin( main_window );
}



/// MenuManager

std::map<std::string, MenuPage *> MenuManager::menu_pages;
MenuPage *MenuManager::current_page = nullptr;
int MenuManager::selected_index = 0;
bool MenuManager::is_on_menu = true;

void MenuManager::initialize_menu_manager() {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();

    keypad( stdscr, TRUE );

    initialize_windows();
}

void MenuManager::initialize_windows() {

    // Initialize windows
    const int main_window_rows = 24;
    const int main_window_cols = 80;
    const int main_window_top  = ( LINES - 25 ) / 2;
    const int main_window_left = ( COLS - 81 ) / 2;

    const int form_window_rows = main_window_rows - 10;
    const int form_window_cols = main_window_cols - 2;
    const int form_window_top  = 1;
    const int form_window_left = 1;

    const int menu_window_rows = main_window_rows - form_window_rows - 2;
    const int menu_window_cols = main_window_cols - 2;
    const int menu_window_top  = form_window_top + form_window_rows;
    const int menu_window_left = 1;

    MenuPage::main_window = newwin( main_window_rows, main_window_cols, 
                                   main_window_left, main_window_top );
    box( MenuPage::main_window, 0, 0 );
    MenuPage::form_window = derwin( MenuPage::main_window, 
                                   form_window_rows, form_window_cols, 
                                   form_window_top, form_window_left );
	box( MenuPage::form_window, 0, 0 );
    MenuPage::menu_window = derwin( MenuPage::main_window, 
                                   menu_window_rows, menu_window_cols, 
                                   menu_window_top, menu_window_left );
	box( MenuPage::menu_window, 0, 0 );
}

void MenuManager::add_menu_page( std::string &page_name, MenuPage *page ) {
    menu_pages[page_name] = page;
}

std::map<std::string, MenuPage *>MenuManager::get_menu_pages() {
    return menu_pages;
}

void MenuManager::set_current_page( MenuPage *page ) {
    current_page = page;
}

MenuPage *MenuManager::get_current_page() {
    return current_page;
}

int MenuManager::get_selected_index() {
    return selected_index;
}

void MenuManager::switch_page( std::string &next_page_name ) {
    unpost_form( current_page->get_form() );
    unpost_menu( current_page->get_menu() );
    set_current_page( menu_pages[next_page_name] );
    post_form( current_page->get_form() );
    post_menu( current_page->get_menu() );
    refresh();
    wrefresh( MenuPage::main_window );
    wrefresh( MenuPage::menu_window );
    wrefresh( MenuPage::form_window );
}

void MenuManager::initialize_starting_page() {
    post_form( current_page->get_form() );
    post_menu( current_page->get_menu() );
    refresh();
    wrefresh( MenuPage::main_window );
    wrefresh( MenuPage::menu_window );
    wrefresh( MenuPage::form_window );
}

void MenuManager::main_menu_driver() {
    // Menu loop
    switch ( current_page->get_selected_option() ) {
        case KEY_DOWN:
            if ( is_on_menu ) {
                if ( selected_index < current_page->get_item_names().size() - 1 ) {
                    menu_driver( current_page->get_menu(), REQ_DOWN_ITEM );
                    selected_index++;
                    wrefresh( MenuPage::menu_window );
                }
            } 
            else {
                if ( selected_index < current_page->get_field_names().size() - 1 ) {
                    form_driver( current_page->get_form(), REQ_NEXT_FIELD );
                    selected_index++;
                    wrefresh( MenuPage::form_window );
                }
                else {
                    // Switch down to menu
                    selected_index = 0;
                    is_on_menu = true;
                    menu_driver( current_page->get_menu(), REQ_FIRST_ITEM );
                    set_menu_fore( current_page->get_menu(), A_REVERSE );
                    wrefresh( MenuPage::form_window );
                }
            }
            break;

        case KEY_UP:
            if ( is_on_menu ) {
                if ( selected_index > 0 ) {
                    selected_index--;
                    menu_driver( current_page->get_menu(), REQ_UP_ITEM );
                } 
                else if ( current_page->has_form() ) {
                    // Switch up to form
                    selected_index = current_page->get_field_names().size() - 1;
                    is_on_menu = false;
                    set_menu_fore( current_page->get_menu(), A_NORMAL );
                    form_driver( current_page->get_form(), REQ_LAST_FIELD );
                }
                wrefresh( MenuPage::menu_window );
            } 
            else {
                if ( selected_index > 0 ) {
                    selected_index--;
                    form_driver( current_page->get_form(), REQ_PREV_FIELD );
                    wrefresh( MenuPage::form_window );
                }
            }
            break;

        case KEY_BACKSPACE:
            if ( !is_on_menu ) {
                form_driver( current_page->get_form(), REQ_DEL_PREV );
                wrefresh( MenuPage::form_window );
            }
            break;

        case KEY_RIGHT:
            if ( !is_on_menu ) {
                form_driver( current_page->get_form(), REQ_NEXT_CHAR );
                wrefresh( MenuPage::form_window );
            }
            break;
        
        case KEY_LEFT:
            if ( !is_on_menu ) {
                form_driver( current_page->get_form(), REQ_PREV_CHAR );
                wrefresh( MenuPage::form_window );
            }
            break;

            case '\n':
            // Menu item chosen. Call corresponding function 
            if ( is_on_menu ) {
                menu_driver( current_page->get_menu(), REQ_FIRST_ITEM );
                current_page->
                    get_item_results()[selected_index]();
                selected_index = 0;
            }
            break;

        default:
            if ( !is_on_menu ) {
                form_driver( current_page->get_form(), 
                            current_page->get_selected_option() );
                wrefresh( MenuPage::form_window );
            }

            break;
    }
    wrefresh( MenuPage::main_window );
}

void MenuManager::cleanup() {
    
    for ( const auto &page : menu_pages ) {
        page.second->cleanup();
    }

    endwin();
}