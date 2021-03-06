// Generated by dia2code
#ifndef AI__SCORE__H
#define AI__SCORE__H

#include <vector>
#include <ostream>

namespace state {
  class State;
};
namespace engine {
  class MoveCommands;
  class AttackCommand;
};
namespace state {
  class Character;
};
namespace ai {
  class Score;
}

#include "state/State.h"
#include "state/Character.h"
#include "engine/MoveCommands.h"
#include "engine/AttackCommand.h"

namespace ai {

  /// class Score - 
  class Score {
    // Associations
    // Attributes
  public:
    int bonusDmgEnnemy     = 0;
    int malusDmgAlly     = 0;
    int bonusCloser     = 0;
    int malusPaUsed     = 0;
    int malusPmUsed     = 0;
    // Operations
  public:
    void setScoreAction (state::State* state, engine::MoveCommands* mv, engine::AttackCommand* atk, state::Character* character, std::vector<state::Character*> allies, std::vector<state::Character*> ennemies);
    int getScore () const;
    friend std::ostream& operator<< (std::ostream& os, const Score& score);
    // Setters and Getters
  };

};

#endif
