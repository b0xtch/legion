/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "ChatWindow.h"

#include <cassert>

#include <form.h>
#include <ncurses.h>

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// Hidden ChatWindow implementation
////////////////////////////////////////////////////////////////////////////////

class ChatWindowImpl {
public:
  ChatWindowImpl(std::function<void(std::string)> onTextEntry, int updateDelay);
  ~ChatWindowImpl();
  ChatWindowImpl(ChatWindowImpl&) = delete;
  ChatWindowImpl(ChatWindowImpl&&) = delete;
  ChatWindowImpl& operator=(ChatWindowImpl&) = delete;
  ChatWindowImpl& operator=(ChatWindowImpl&&) = delete;

  void processInput(int key);

  [[nodiscard]] size_t getFieldSize() const;

  [[nodiscard]] std::string getFieldString() const;

  void refreshWindow();

  void displayText(const std::string& text);

private:
  std::function<void(std::string)> onTextEntry;

  int positionTop  = 0;
  int positionLeft = 0;
  int rows = 0;
  int columns = 0;
  int entrySize = 3;

  WINDOW *view     = nullptr;
  WINDOW *entry    = nullptr;
  WINDOW *entrySub = nullptr;

  FIELD *fields[2]  = { nullptr, nullptr };
  FIELD *entryField = nullptr;

  FORM *entryForm = nullptr;
};


ChatWindowImpl::ChatWindowImpl(std::function<void(std::string)> onTextEntry, int updateDelay)
  : onTextEntry{std::move(onTextEntry)} {

  halfdelay(updateDelay);

  // TEMP hardcoded values
  const int positionTop = 1;
  const int positionLeft = 40 + 1;
  //getmaxyx(view, positionTop, positionLeft);
  const int rows = 24 - 2;
  const int columns = (80 / 2) - 2;

  view = newwin(rows - entrySize, columns, positionTop, positionLeft);

  scrollok(view, TRUE);

  entry = newwin(entrySize, columns, positionTop + rows - entrySize, positionLeft);
  wborder(entry, ' ', ' ', '-', ' ', '+', '+', ' ', ' ');
  entrySub = derwin(entry, entrySize - 1, columns, 1, 0);
  
  entryField = new_field(entrySize - 1, columns, 0, 0, 0, 0);
  assert(entryField && "Error creating entry field.");
  set_field_buffer(entryField, 0, "");
  set_field_opts(entryField, O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

  fields[0] = entryField;
  entryForm = new_form(fields);
  assert(entryForm && "Error creating entry form.");
  set_form_win(entryForm, entry);
  set_form_sub(entryForm, entrySub);
  post_form(entryForm);
  form_driver( entryForm, REQ_FIRST_FIELD );

  refresh();
  wrefresh(entry);
}


ChatWindowImpl::~ChatWindowImpl() {
  unpost_form(entryForm);
  free_form(entryForm);
  free_field(entryField);
  delwin(entry);
  delwin(view);
  endwin();
}


void
ChatWindowImpl::processInput(int key) {

  switch(key) {
    case KEY_ENTER:
    case '\n':
      // Requesting validation synchs the seen field & the buffer.
      form_driver(entryForm, REQ_VALIDATION);
      onTextEntry(getFieldString());
      displayText(getFieldString() + "\n");
      move(1, 1);
      set_field_buffer(entryField, 0, "");
      refresh();
      pos_form_cursor(entryForm);
      break;
    case KEY_BACKSPACE:
    case 127: //ASCII delete
      form_driver(entryForm, REQ_DEL_PREV);
      break;
    case KEY_DC:
      form_driver(entryForm, REQ_DEL_CHAR);
      break;
    case ERR:
      // swallow
      break;
    default:
      form_driver(entryForm, key);
      break;
  }
}


void
ChatWindowImpl::refreshWindow() {
  wrefresh(view);
  wrefresh(entry);
}


void
ChatWindowImpl::displayText(const std::string& text) {
  // This variadic function is part of the curses interface.
  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-vararg)
  wprintw(view, "%s", text.c_str());
}


size_t
ChatWindowImpl::getFieldSize() const {
  size_t x, y;
  getyx(entrySub, y, x);
  return y * columns + x;
}


std::string
ChatWindowImpl::getFieldString() const {
  return std::string{field_buffer(entryField, 0), getFieldSize()};
}


////////////////////////////////////////////////////////////////////////////////
// ChatWindow API
////////////////////////////////////////////////////////////////////////////////


ChatWindow::ChatWindow(std::function<void(std::string)> onTextEntry, int updateDelay)
  : impl{std::make_unique<ChatWindowImpl>(std::move(onTextEntry), updateDelay)}
    { }


ChatWindow::~ChatWindow() = default;


void
ChatWindow::update() {
  // TODO: Add some state variable for only processing input when the chatWindow is selected
  impl->processInput(getch());
  impl->refreshWindow();
}


void
ChatWindow::displayText(const std::string& text) {
  impl->displayText(text);
}