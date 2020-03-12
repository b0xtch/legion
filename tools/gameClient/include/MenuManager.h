#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <vector>
#include <functional>
#include <map>
#include <menu.h>
#include <form.h>
#include <memory>
#include "ChatWindow.h"


class MenuManager {

public:

    enum CursorPosition {
        menu,
        form, 
        chat
    };

    MenuManager(ChatWindow *chatWindow);

    void initializeWindows();
    void initializeStartingPage();
    void addPage( std::shared_ptr<MenuPage> page );
    void setCurrentPage( std::shared_ptr<MenuPage> page );
    std::shared_ptr<MenuPage> getCurrentPage();
    int getSelectedIndex();
    void switchPage( MenuPage::MenuName &next_page_name );
    void switchCursorPosition( CursorPosition newPosition );
    void processInput();
    void processMenuInput();
    void processFormInput();
    void processCommonInput();
    void refreshWindows();
    void update();
    void displayChatText(const std::string& text);
    void cleanup();

private:

    WINDOW *mainWindow;
    WINDOW *formWindow;
    WINDOW *menuWindow;

    std::map<MenuPage::MenuName, std::shared_ptr<MenuPage>> menuPages;
    std::shared_ptr<MenuPage> currentPage;
    CursorPosition cursorPosition = CursorPosition::menu;
    int selectedIndex;

    ChatWindow *chatWindow;

    int keyPress;

};

#endif