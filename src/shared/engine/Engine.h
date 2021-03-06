// Generated by dia2code
#ifndef ENGINE__ENGINE__H
#define ENGINE__ENGINE__H

#include <deque>
#include <stdlib.h>

namespace engine {
  class Command;
}

#include "Command.h"

namespace engine {

  /// class Engine - 
  class Engine {
    // Associations
    // Attributes
  private:
    std::deque<Command*> commands;
    std::deque<Command*> rollback;
    bool reverse     = false;
    bool reverse_tmp     = false;
    // Operations
  public:
    void addCommand (Command* command);
    void runCommand ();
    Command* getCommand ();
    std::deque<Command*> getCommands (bool b = false);
    void clearCommand ();
    void clearCommands (bool b = false);
    std::size_t getSize ();
    bool getReverse ();
    void toggleReverse ();
    // Setters and Getters
  };

};

#endif
