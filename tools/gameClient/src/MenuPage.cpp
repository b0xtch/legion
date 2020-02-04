#include <string>
#include "../include/MenuPage.h"

WINDOW * MenuPage::main_window = nullptr;
WINDOW * MenuPage::form_window = nullptr;
WINDOW * MenuPage::menu_window = nullptr;

MenuPage::MenuPage(std::string menu_name,
                   std::vector<const char *> fields,
                   std::vector<const char *> options,
                   std::vector<void (*)(std::map<std::string, MenuPage *>,
                                        MenuPage **current_page)> option_results) {

    this->menu_name = menu_name;
    this->fields = fields;
    this->options = options;
    this->option_results = option_results;

    const int margin_left = 20;
    const int margin_top = 2;

    // Initialize forms
    if (has_form()) {
        field_size = (sizeof(fields) / sizeof(fields[0]));
        field_list = (FIELD **)calloc(field_size + 1, sizeof(FIELD *));

        const int margin_left_field = 5;
        for (int i = 0; i < field_size; i++) {
            bool is_input_field = i % 2 == 1;

            if (is_input_field) {
                field_list[i] = new_field(1, 20, margin_top, margin_left, 0, 0);
                field_opts_on(field_list[i], O_ACTIVE);
                field_opts_on(field_list[i], O_EDIT);
                set_field_back(field_list[i], A_UNDERLINE); 
            } else {
                field_list[i] = new_field(1, 20, margin_top, margin_left_field, 0, 0);
                field_opts_off(field_list[i], O_ACTIVE);
                field_opts_off(field_list[i], O_EDIT);
            }

            set_field_buffer(field_list[i], 0, fields[i]);
        }

        form = new_form((FIELD **)field_list);
        set_form_win(form, form_window);
        WINDOW *form_inner = derwin(form_window, form->rows + 1, form->cols + 1, 1, 1);
        set_form_sub(form, form_inner);
    }

    // Initialize menu
    options_size = (sizeof(options) / sizeof(options[0]));
    item_list = (ITEM **)calloc(options_size + 1, sizeof(ITEM *));

    for (int i = 0; i < options_size; i++) {
        item_list[i] = new_item(options[i], options[i]);
    }

    item_list[options_size] = (ITEM *)NULL;
    menu = new_menu((ITEM **)item_list);

    int max_menu_item_length = 0;
    for (std::string option : options) {
        if (option.length() > max_menu_item_length) {
            max_menu_item_length = option.length() + 1;
        }
    }
	set_menu_win(menu, menu_window);
	WINDOW *menu_inner = derwin(menu_window, options_size, max_menu_item_length, 
                                margin_top, margin_left);
	set_menu_sub(menu, menu_inner);
}

std::vector<const char *> MenuPage::get_fields() {
    return fields;
}

FORM* MenuPage::get_form() {
    return form;
}

bool MenuPage::has_form() {
    return !fields.empty();
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

void MenuPage::cleanup() {
    unpost_form(form);
    for (int i = 0; i < options_size; i++) {
        free_item(item_list[i]);
    }
    free_menu(menu);

    unpost_menu(menu);
    for (int i = 0; i < field_size; i++) {
        free_field(field_list[i]);
    }
    free_form(form);

	delwin(form_window);
	delwin(menu_window);
	delwin(main_window);
}




