// Generated by dia2code
#ifndef AI__DEEPAI__H
#define AI__DEEPAI__H

#include <memory>
#include <vector>

namespace ai {
  class TreeNode;
};
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
  class AI;
}

#include "TreeNode.h"
#include "AI.h"

namespace ai {

  /// class DeepAI - 
  class DeepAI : public ai::AI {
    // Associations
    // Attributes
  private:
    std::shared_ptr<TreeNode> rootNode;
    std::shared_ptr<TreeNode> nodeToRun;
    int scoremax     = 0;
    // Operations
  public:
    DeepAI (state::State* state, engine::Engine* engine);
    std::vector<std::tuple<engine::MoveCommands*, engine::AttackCommand*,Score>> getBestActions (state::Character* character, int threshold = 0);
    void buildTree (std::shared_ptr<TreeNode> node, int depth, int teamNumber = 1);
    std::vector<state::Character*> getTurnOrder (std::vector<state::Character*> characters);
    void run (state::Character* character);
    // Setters and Getters
  };

};

#endif
