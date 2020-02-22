#include <string>
#include "MenuPage.h"
#include "MenuManager.h"

#include <iostream>
#include <assert.h>

MenuManager::MenuManager() : selected_index(0), is_on_menu(true) {

    // Initialize ncurses
    initscr();
    noecho();
    cbreak();

    keypad( stdscr, TRUE );

    initialize_windows();
}

void MenuManager::add_page( MenuPage *page ) {

    const int margin_left = 20;
    const int margin_top = 2;

    // Initialize forms
    if ( page->has_form() ) {
        const int margin_left_field = 5;
        int field_index = 0;

        for ( int i = 0; i < page->get_field_names().size() * 2; i++ ) {
            bool is_field_name = i % 2 == 0;

            if ( is_field_name ) {
                // Add field name to form
                page->add_field( new_field( 1, 20, margin_top + field_index * 2, 
                                               margin_left_field, 0, 0 ) );
                field_opts_off( page->get_field_list()->at( i ), O_ACTIVE );
                field_opts_off( page->get_field_list()->at( i ), O_EDIT );
                set_field_buffer( page->get_field_list()->at( i ), 0,
                                  page->get_field_names()[field_index] );
                field_index++;
            } 
            else {
                // Add field entry to form
                page->add_field( new_field( 1, 20, margin_top + i - 1, 
                                          margin_left, 0, 0 ) );
                field_opts_on( page->get_field_list()->at( i ), O_ACTIVE );
                field_opts_on( page->get_field_list()->at( i ), O_EDIT );
                set_field_back( page->get_field_list()->at( i ), A_UNDERLINE ); 
                set_field_buffer( page->get_field_list()->at( i ), 0, "" );           
            }
        }

        page->add_field( nullptr );

        page->set_form( new_form( page->get_field_list()->data() ) );
        set_form_win( page->get_form(), form_window );
        WINDOW *form_inner = derwin( form_window, 
                                     page->get_form()->rows + 1, 
                                     page->get_form()->cols + 1, 
                                     1, 1 );
        set_form_sub( page->get_form(), form_inner );

    } 

    // Initialize menu
    for ( auto &item_name : page->get_item_names() ) {
        page->add_item( new_item( item_name, item_name ) );
    }
    page->add_item( nullptr );

    page->set_menu( new_menu( page->get_item_list()->data() ) );

    int max_menu_item_length = 0;
    for ( std::string item_name : page->get_item_names() ) {
        if ( item_name.length() > max_menu_item_length ) {
            max_menu_item_length = item_name.length() + 1;
        }
    }
	set_menu_win( page->get_menu(), menu_window );
	WINDOW *menu_inner = derwin( menu_window, page->get_item_names().size(), 
                                 max_menu_item_length, margin_top, margin_left );
	set_menu_sub( page->get_menu(), menu_inner );

    menu_pages[page->get_menu_name()] = page;

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

    main_window = newwin( main_window_rows, main_window_cols, 
                          main_window_left, main_window_top );
    box( main_window, 0, 0 );
    form_window = derwin( main_window, 
                          form_window_rows, form_window_cols, 
                        form_window_top, form_window_left );
	box( form_window, 0, 0 );
    menu_window = derwin( main_window, 
                          menu_window_rows, menu_window_cols, 
                          menu_window_top, menu_window_left );
	box( menu_window, 0, 0 );
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

    menu_driver( current_page->get_menu(), REQ_FIRST_ITEM );
    selected_index = 0;

    if ( current_page->has_form() ) {
        unpost_form( current_page->get_form() );
    }
    
    unpost_menu( current_page->get_menu() );

    set_current_page( menu_pages[next_page_name] );

    if ( current_page->has_form() ) {
        assert( post_form( current_page->get_form() ) == E_OK );
    }
    assert( post_menu( current_page->get_menu() ) == E_OK );

    refresh();
    wrefresh( main_window );
    wrefresh( menu_window );
    wrefresh( form_window );
    
}

void MenuManager::initialize_starting_page() {

    if ( current_page->has_form() ) {
        assert( post_form( current_page->get_form() ) == E_OK );
    }
    assert( post_menu( current_page->get_menu() ) == E_OK );

    refresh();
    wrefresh( main_window );
    wrefresh( menu_window );
    wrefresh( form_window );
}

void MenuManager::main_menu_driver() {

    current_page->change_selected_option_on_input();

    // Menu loop
    switch ( current_page->get_selected_option() ) {
        case KEY_DOWN:
            if ( is_on_menu ) {
                if ( selected_index < current_page->get_item_names().size() - 1 ) {
                    menu_driver( current_page->get_menu(), REQ_DOWN_ITEM );
                    selected_index++;
                    wrefresh( menu_window );
                }
            } 
            else {
                if ( selected_index < current_page->get_field_names().size() - 1 ) {
                    form_driver( current_page->get_form(), REQ_NEXT_FIELD );
                    selected_index++;
                    wrefresh( form_window );
                }
                else {
                    // Switch down to menu
                    selected_index = 0;
                    is_on_menu = true;
                    menu_driver( current_page->get_menu(), REQ_FIRST_ITEM );
                    set_menu_fore( current_page->get_menu(), A_REVERSE );
                    wrefresh( form_window );
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
                wrefresh( menu_window );
            } 
            else {
                if ( selected_index > 0 ) {
                    selected_index--;
                    form_driver( current_page->get_form(), REQ_PREV_FIELD );
                    wrefresh( form_window );
                }
            }
            break;

        case KEY_BACKSPACE:
            if ( !is_on_menu ) {
                form_driver( current_page->get_form(), REQ_DEL_PREV );
                wrefresh( form_window );
            }
            break;

        case KEY_RIGHT:
            if ( !is_on_menu ) {
                form_driver( current_page->get_form(), REQ_NEXT_CHAR );
                wrefresh( form_window );
            }
            break;
        
        case KEY_LEFT:
            if ( !is_on_menu ) {
                form_driver( current_page->get_form(), REQ_PREV_CHAR );
                wrefresh( form_window );
            }
            break;

            case '\n':
            // Menu item chosen. Call corresponding function 
            if ( is_on_menu ) {
                current_page->
                    get_item_results()[selected_index]();
            }
            break;

        default:
            if ( !is_on_menu ) {
                form_driver( current_page->get_form(), 
                            current_page->get_selected_option() );
                wrefresh( form_window );
            }

            break;
    }
    wrefresh( main_window );
}

void MenuManager::cleanup() {
    
    for ( const auto &page : menu_pages ) {
        page.second->cleanup();
    }

    delwin( form_window );
	delwin( menu_window );
	delwin( main_window );

    endwin();
}