// Generated by dia2code
#ifndef STATE__KILLQUEST__H
#define STATE__KILLQUEST__H

#include <map>

namespace state {
  class MainQuest;
}

#include "MainQuest.h"
#include "CharacterType.h"

namespace state {

  /// class KillQuest - 
  class KillQuest : public state::MainQuest {
    // Associations
    // Attributes
  private:
    std::map<CharacterType,int> goal;
    std::map<CharacterType,int> kills;
    // Operations
  public:
    KillQuest ();
    // Setters and Getters
  };

};

#endif
