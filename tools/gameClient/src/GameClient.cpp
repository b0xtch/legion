#include <iostream>
#include <ncurses.h>
#include <menu.h>
#include <form.h>

#include "../include/MenuPage.h"
#include "../../../lib/networking/include/Client.h"

using namespace std;

// Basic menu structure referenced from: 
//    http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/menus.html

// Basic menu structure referenced from: 
//    http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/forms.html

// Idea on combining menus and forms on one page referenced from:
//    https://alan-mushi.github.io/2015/05/26/simple-ncurses-popup-in-C.html


// Menu item results functions
void connect_to_server();
void switch_to_main_menu_page();
void switch_to_join_lobby_page();
void switch_to_create_lobby_page();
void exit_program();

// Make json message for server
string makeServerMessage(string input);

int main(int argc, char* argv[]) {

    MenuManager::initialize_menu_manager();

    // Initialize menu pages

    // Connect menu
    MenuPage::NameList connect_menu_fields
        = {"Server IP:", "Server port:"};

    MenuPage::NameList connect_menu_items
        = {"Connect"};

    const MenuPage::FunctionList connect_menu_item_results
        = {connect_to_server};

    string connect_menu_name = "Connect menu";
    MenuPage *connect_menu_page = new MenuPage( connect_menu_name,
                                               connect_menu_fields,
                                               connect_menu_items, 
                                               connect_menu_item_results );
    MenuManager::add_menu_page( connect_menu_name, connect_menu_page );


    // Main menu
    MenuPage::NameList main_menu_fields = {};

    MenuPage::NameList main_menu_items 
        = {"Join lobby", "Create lobby", "Exit"};

    const MenuPage::FunctionList main_menu_item_results
        = {switch_to_join_lobby_page, switch_to_create_lobby_page, exit_program};

    string main_menu_name = "Main menu";
    MenuPage *main_menu_page = new MenuPage( main_menu_name,
                                            main_menu_fields, 
                                            main_menu_items, 
                                            main_menu_item_results );
    MenuManager::add_menu_page( main_menu_name, main_menu_page );


    // Join lobby menu
    MenuPage::NameList join_lobby_fields
        = {"Lobby code:"};

    MenuPage::NameList join_lobby_items
        = {"Join", "Back"};

    const MenuPage::FunctionList join_lobby_item_results
        = {switch_to_main_menu_page, switch_to_main_menu_page};

    string join_lobby_name = "Join lobby";
    MenuPage *join_lobby_page = new MenuPage( join_lobby_name,
                                             join_lobby_fields,
                                             join_lobby_items, 
                                             join_lobby_item_results );
    MenuManager::add_menu_page( join_lobby_name, join_lobby_page );


    // Create lobby menu
    MenuPage::NameList create_lobby_fields
        = {"Lobby code:"};
    MenuPage::NameList create_lobby_items
        = {"Game 1", "Game 2", "Game 3", "Back"};

    const MenuPage::FunctionList create_lobby_item_results
        = {switch_to_main_menu_page, switch_to_main_menu_page, 
           switch_to_main_menu_page, switch_to_main_menu_page
           }; 

    string create_lobby_name = "Create lobby";
    MenuPage *create_lobby_page = new MenuPage( create_lobby_name,
                                               create_lobby_fields, 
                                               create_lobby_items, 
                                               create_lobby_item_results );  
    MenuManager::add_menu_page( create_lobby_name, create_lobby_page ); 

    MenuManager::set_current_page( connect_menu_page );

    // Start menu

    MenuManager::main_loop();

    MenuManager::cleanup();

    // ----------------------------------------------------
    // main loop

    if (argc < 3) {
        // Give user message about incorrect arguments
        return 1;
    }

    networking::Client client{argv[1], argv[2]};

    bool done = false;

    string userInput;
    // Menu gets user input and sets the above variable
    client.send( makeServerMessage(userInput) );

    while (!done && !client.isDisconnected()) {
        try {
            client.update();
        } catch (std:exception& e) {
            // Give user message about exception
            done = true;
        }
    
        auto response = client.receive();
        if (!response.empty()) {
            // Give user response
        }

    }

    return 0;
}

void connect_to_server() {
    // TODO: connect to server with given ip and port in fields
    MenuPage *connect_page = MenuManager::get_current_page();
    vector<FIELD *> connect_fields = connect_page->get_field_list();

    const int server_ip_input_index = 1;
    char *server_ip = field_buffer( connect_fields[server_ip_input_index], 0 );
    const int server_port_input_index = 3;
    char *server_port = field_buffer( connect_fields[server_port_input_index], 0 );

    networking::Client client{server_ip, server_port};

    switch_to_main_menu_page();
}

void switch_to_join_lobby_page() {
    string next_page_name = "Join lobby";
    MenuManager::switch_page( next_page_name );
}

void switch_to_create_lobby_page() {
    string next_page_name = "Create lobby";
    MenuManager::switch_page( next_page_name );
}

void switch_to_main_menu_page() {
    string next_page_name = "Main menu";
    MenuManager::switch_page( next_page_name );
}

void exit_program() {
    
}

/** 
Takes a string input and converts it into a json object consisting of 2 fields.
The first field is the command to be given to the server. The second field is any extra arguments for the command.

If the first word of the input matches a possible command, it is set as the value for the command field.
Then the rest of the input is put in the data field.

If the first word does not match a possible command, the input is assumed to be a chat message.
The command is set to !chat, and the entire input is put in the data field.
**/
string makeServerMessage(string input) {
    
    vector<string> possibleCommands;
    possibleCommands.push_back("!createsession");
    possibleCommands.push_back("!joinsession");

    string command = "{ \"command\": \"";
    
    size_t endOfCommand = input.find(" ");
    string firstWord = input.substr(0,endOfCommand);
    if (firstWord == "!example") {
        command += firstWord;
    } else {
        command += "!chat";
        endOfCommand = 0;
    }

    string data = "\", \"data\": \"";
    data += input.substr(endOfCommand, string::npos);

    auto message = json::parse( command+data+"\" }" );

    return message.dump();

}

