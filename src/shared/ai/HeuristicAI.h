// Generated by dia2code
#ifndef AI__HEURISTICAI__H
#define AI__HEURISTICAI__H

#include <vector>

namespace state {
  class State;
};
namespace engine {
  class Engine;
};
namespace state {
  class Character;
};
namespace ai {
  class Score;
};
namespace engine {
  class MoveCommands;
  class AttackCommand;
};
namespace ai {
  class AI;
}

#include "Score.h"
#include "AI.h"

namespace ai {

  /// class HeuristicAI - 
  class HeuristicAI : public ai::AI {
    // Operations
  public:
    HeuristicAI (state::State* state, engine::Engine* engine);
    std::tuple<engine::MoveCommands*, engine::AttackCommand*> getBestAction (state::Character* character);
    std::vector<state::Character*> getTurnOrder (std::vector<state::Character*> characters);
    void run (state::Character* character);
  private:
    bool isCharacterAtpos (state::Character* c, int i, int j);
    Score getScoreAction (engine::MoveCommands* mv, engine::AttackCommand* atk, state::Character* character, std::vector<state::Character*> allies, std::vector<state::Character*> ennemies);
    // Setters and Getters
  };

};

#endif
