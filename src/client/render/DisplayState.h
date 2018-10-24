// Generated by dia2code
#ifndef RENDER__DISPLAYSTATE__H
#define RENDER__DISPLAYSTATE__H


namespace state {
  class State;
};
namespace render {
  class CharacterSprite;
  class TileSprite;
}

#include "CharacterSprite.h"
#include "TileSprite.h"
#include "state/State.h"

namespace render {

  /// class DisplayState - 
  class DisplayState {
    // Associations
    // Attributes
  private:
    state::State* state;
    // Operations
  public:
    DisplayState (state::State* state);
    void display ();
    // Setters and Getters
  };

};

#endif
