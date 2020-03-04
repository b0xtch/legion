#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <vector>
#include <functional>
#include <map>
#include <menu.h>
#include <form.h>
#include "ChatWindow.h"


class MenuManager {

public:

    MenuManager(std::function<void(std::string)> onTextEntry);

    void initializeWindows(std::function<void(std::string)> onTextEntry);
    void initializeStartingPage();
    void addPage( MenuPage *page );
    void setCurrentPage( MenuPage *page );
    MenuPage *getCurrentPage();
    int getSelectedIndex();
    void switchPage( MenuPage::MenuName &next_page_name );
    void processInput();
    void processMenuInput();
    void processFormInput();
    void refreshWindows();
    void update();
    void cleanup();

private:

    WINDOW *mainWindow;
    WINDOW *formWindow;
    WINDOW *menuWindow;

    std::map<MenuPage::MenuName, MenuPage *> menuPages;
    MenuPage *currentPage;
    int selectedIndex;
    bool isOnMenu;

    ChatWindow *chatWindow;

};

#endif