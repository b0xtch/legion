/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <functional>
#include <memory>
#include <ncurses.h>


// Note: The ncurses state needs to be hidden from the public interface
// due to macro conflicts with boost asio. Thus, we hide the implementation
// requiring state behind indirection for a compilation firewall.
class ChatWindowImpl;

namespace ChatWindowInfo {
  struct Position {
    Position(int x, int y) : x(x), y(y) {};
    int x;
    int y;
  };

  struct Dimensions {
    Dimensions(int rows, int columns) : rows(rows), columns(columns) {};
    int rows;
    int columns;
  };
}

class ChatWindow {
public:
  ChatWindow( std::function<void(std::string)> onTextEntry, 
              ChatWindowInfo::Position position,
              ChatWindowInfo::Dimensions dimensions)
    : ChatWindow{std::move(onTextEntry), position, dimensions, 1}
      { }

  ChatWindow( std::function<void(std::string)> onTextEntry )
    : ChatWindow{std::move(onTextEntry), 
      ChatWindowInfo::Position{5, 5},
      ChatWindowInfo::Dimensions{5, 38}, 1}
      { }

      

  ChatWindow( std::function<void(std::string)> onTextEntry, 
              ChatWindowInfo::Position position,
              ChatWindowInfo::Dimensions dimensions,
              int updateDelay);
  // The default constructor is out of line to defer it until the implementation
  // of ChatWindowImpl is known.
  ~ChatWindow();

  void update();

  void displayText(const std::string& text);

  void activate();
  void deactivate();

  int getKeyPress();

  void moveAndScale(ChatWindowInfo::Position position, ChatWindowInfo::Dimensions dimensions);

  void refreshWindow();

private:
  std::unique_ptr<ChatWindowImpl> impl;
};


#endif

