#include <string>
#include "MenuPage.h"

MenuPage::MenuPage(std::string menu_name,
                   std::vector<const char *> options, 
                   std::vector<void (*)(std::map<std::string, MenuPage *>, 
                                        MenuPage **current_page)> option_results) {

    this->menu_name = menu_name;
    this->options = options;
    this->option_results = option_results;

    options_size = (sizeof(options) / sizeof(options[0]));
    item_list = (ITEM **)calloc(options_size + 1, sizeof(ITEM *));

    for (int i = 0; i < options_size; i++) {
        item_list[i] = new_item(options[i], options[i]);
    }

    item_list[options_size] = (ITEM *)NULL;

    menu = new_menu((ITEM **)item_list);
}

std::vector<const char *> MenuPage::get_options() {
    return options;
}

MENU* MenuPage::get_menu() {
    return menu;
}

int MenuPage::get_current_option() {
    return current_option;
}

int MenuPage::change_current_option_on_input() {
    current_option = getch();
    return current_option;
}

std::vector<void (*)(std::map<std::string, MenuPage *>, 
            MenuPage **current_page)> MenuPage::get_option_results() {
    return option_results;
}

int MenuPage::get_options_size() {
    return options_size;
}

MenuPage::~MenuPage() {
    for (int i = 0; i < options_size; i++) {
        free_item(item_list[i]);
    }
    free_menu(menu);
}