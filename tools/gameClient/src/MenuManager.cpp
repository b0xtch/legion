#include "MenuPage.h"
#include "ChatWindow.h"
#include "MenuManager.h"

#include <string_view>
#include <assert.h>

#include <iostream>

MenuManager::MenuManager(std::function<void(std::string)> onTextEntry) : selectedIndex(0), isOnMenu(true) {

    // Initialize ncurses
    initscr();
    noecho();
    cbreak();

    keypad( stdscr, TRUE );

    initializeWindows(onTextEntry);
}

void MenuManager::addPage( MenuPage *page ) {

    const int marginLeft = 3;
    const int marginTop = 2;

    // Initialize forms
    if ( page->hasForm() ) {
        const int marginLeftField = 3;
        const int maxFieldLength = 15;
        int fieldIndex = 0;

        for ( int i = 0; i < page->getFieldNames().size() * 2; i++ ) {
            bool isFieldName = i % 2 == 0;

            if ( isFieldName ) {
                // Add field name to form
                page->addField( new_field( 1, maxFieldLength, marginTop + fieldIndex * 2, 
                                               marginLeftField, 0, 0 ) );
                field_opts_off( page->getFieldList()->at( i ), O_ACTIVE );
                field_opts_off( page->getFieldList()->at( i ), O_EDIT );
                set_field_buffer( page->getFieldList()->at( i ), 0,
                                  page->getFieldNames()[fieldIndex] );
                fieldIndex++;
            } 
            else {
                // Add field entry to form
                page->addField( new_field( 1, maxFieldLength, marginTop + i - 1, 
                                          marginLeft + maxFieldLength, 0, 0 ) );
                field_opts_on( page->getFieldList()->at( i ), O_ACTIVE );
                field_opts_on( page->getFieldList()->at( i ), O_EDIT );
                set_field_back( page->getFieldList()->at( i ), A_UNDERLINE ); 
                set_field_buffer( page->getFieldList()->at( i ), 0, "" );           
            }
        }

        page->addField( nullptr );

        page->setForm( new_form( page->getFieldList()->data() ) );
        set_form_win( page->getForm(), formWindow );
        WINDOW *formInner = derwin( formWindow, 
                                     page->getForm()->rows + 1, 
                                     page->getForm()->cols + 1, 
                                     1, 1 );
        set_form_sub( page->getForm(), formInner );

    } 

    // Initialize menu
    for ( auto &itemName : page->getItemNames() ) {
        page->addItem( new_item( itemName, itemName ) );
    }
    page->addItem( nullptr );

    page->setMenu( new_menu( page->getItemList()->data() ) );

    int maxMenuItemLength = 0;
    for ( std::string_view itemName : page->getItemNames() ) {
        if ( itemName.length() > maxMenuItemLength ) {
            maxMenuItemLength = itemName.length() + 1;
        }
    }
	set_menu_win( page->getMenu(), menuWindow );
	WINDOW *menuInner = derwin( menuWindow, page->getItemNames().size(), 
                                 maxMenuItemLength, marginTop, marginLeft );
	set_menu_sub( page->getMenu(), menuInner );

    menuPages[page->getMenuName()] = page;

}

void MenuManager::initializeWindows(std::function<void(std::string)> onTextEntry) {

    // Initialize windows
    const int mainWindowRows = 24;
    const int mainWindowCols = 40;
    const int mainWindowTop  = 0;
    const int mainWindowLeft = 0;

    const int formWindowRows = mainWindowRows - 10;
    const int formWindowCols = mainWindowCols - 2;
    const int formWindowTop  = 1;
    const int formWindowLeft = 1;

    const int menuWindowRows = mainWindowRows - formWindowRows - 2;
    const int menuWindowCols = mainWindowCols - 2;
    const int menuWindowTop  = formWindowTop + formWindowRows;
    const int menuWindowLeft = 1;

    const int chatWindowRows = mainWindowRows - 2;
    const int chatWindowCols = mainWindowCols - 2;
    const int chatWindowTop  = 1;
    const int chatWindowLeft = menuWindowCols + 2;

    mainWindow = newwin( mainWindowRows, mainWindowCols, 
                         mainWindowLeft, mainWindowTop );
    box( mainWindow, 0, 0 );
    formWindow = derwin( mainWindow, 
                         formWindowRows, formWindowCols, 
                         formWindowTop, formWindowLeft );
	box( formWindow, 0, 0 );
    menuWindow = derwin( mainWindow, 
                         menuWindowRows, menuWindowCols, 
                         menuWindowTop, menuWindowLeft );
	box( menuWindow, 0, 0 );

    // TEMP
    chatWindow = new ChatWindow(onTextEntry);
}

void MenuManager::setCurrentPage( MenuPage *page ) {
    currentPage = page;
}

MenuPage *MenuManager::getCurrentPage() {
    return currentPage;
}

int MenuManager::getSelectedIndex() {
    return selectedIndex;
}

void MenuManager::switchPage( MenuPage::MenuName &nextPageName ) {

    wclear( mainWindow );
    wclear( formWindow );
    wclear( menuWindow );
    box( mainWindow, 0, 0 );
    box( formWindow, 0, 0 );
    box( menuWindow, 0, 0 );

    //menu_driver( currentPage->getMenu(), REQ_FIRST_ITEM );
    selectedIndex = 0;

    if ( currentPage->hasForm() ) {
        unpost_form( currentPage->getForm() );
    }
    
    unpost_menu( currentPage->getMenu() );

    setCurrentPage( menuPages[nextPageName] );

    if ( currentPage->hasForm() ) {
        assert( post_form( currentPage->getForm() ) == E_OK );
    }
    assert( post_menu( currentPage->getMenu() ) == E_OK );
    
}

void MenuManager::initializeStartingPage() {

    if ( currentPage->hasForm() ) {
        assert( post_form( currentPage->getForm() ) == E_OK );
    }
    assert( post_menu( currentPage->getMenu() ) == E_OK );

    refresh();
    wrefresh( mainWindow );
    wrefresh( menuWindow );
    wrefresh( formWindow );
}

void MenuManager::processInput() {

    currentPage->changeSelectedOptionOnInput();

    if ( isOnMenu ) {
        processMenuInput();
    } else {
        processFormInput();
    }
}

void MenuManager::processMenuInput() {
    switch ( currentPage->getSelectedOption() ) {
        case KEY_DOWN:
            if ( selectedIndex < currentPage->getItemNames().size() - 1 ) {
                menu_driver( currentPage->getMenu(), REQ_DOWN_ITEM );
                selectedIndex++;
            }
            break;

        case KEY_UP:
            if ( selectedIndex > 0 ) {
                selectedIndex--;
                menu_driver( currentPage->getMenu(), REQ_UP_ITEM );
            } 
            else if ( currentPage->hasForm() ) {
                // Switch up to form
                selectedIndex = currentPage->getFieldNames().size() - 1;
                isOnMenu = false;
                set_menu_fore( currentPage->getMenu(), A_NORMAL );
                form_driver( currentPage->getForm(), REQ_LAST_FIELD );
            }
            break;

        case '\n':
            // Menu item chosen. Call corresponding function 
            currentPage->
                getItemResults()[selectedIndex]();
            break;
    }
}

void MenuManager::processFormInput() {
    switch ( currentPage->getSelectedOption() ) {
        case KEY_DOWN:
            if ( selectedIndex < currentPage->getFieldNames().size() - 1 ) {
                form_driver( currentPage->getForm(), REQ_NEXT_FIELD );
                selectedIndex++;
            }
            else {
                // Switch down to menu
                selectedIndex = 0;
                isOnMenu = true;
                menu_driver( currentPage->getMenu(), REQ_FIRST_ITEM );
                set_menu_fore( currentPage->getMenu(), A_REVERSE );
            }
            break;

        case KEY_UP:
            if ( selectedIndex > 0 ) {
                selectedIndex--;
                form_driver( currentPage->getForm(), REQ_PREV_FIELD );
            }
            break;

        case KEY_BACKSPACE:
            form_driver( currentPage->getForm(), REQ_DEL_PREV );
            break;

        case KEY_RIGHT:
            form_driver( currentPage->getForm(), REQ_NEXT_CHAR );
            break;
        
        case KEY_LEFT:
            form_driver( currentPage->getForm(), REQ_PREV_CHAR );
            break;

        default:
            form_driver( currentPage->getForm(), 
                         currentPage->getSelectedOption() );
            break;
    }
}

void MenuManager::refreshWindows() {
    wrefresh(mainWindow);
    wrefresh(formWindow);
    wrefresh(menuWindow);
}

void MenuManager::update() {
  //refreshWindows();
  //processInput();
  //refreshWindows();
  chatWindow->update();
}

void MenuManager::cleanup() {
     
    for ( const auto &page : menuPages ) {
        page.second->cleanup();
    }
   
    delwin( formWindow );
	delwin( menuWindow );
	delwin( mainWindow );

    endwin();
}