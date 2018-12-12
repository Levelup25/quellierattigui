// Generated by dia2code
#ifndef AI__TREENODE__H
#define AI__TREENODE__H

#include <vector>

namespace ai {
  class TreeNode;
};
namespace engine {
  class Command;
};
namespace ai {
  class Score;
}

#include "Score.h"
#include "engine/Command.h"

namespace ai {

  /// class TreeNode - 
  class TreeNode {
    // Associations
    // Attributes
  public:
    TreeNode* parent;
    std::vector<TreeNode*> children;
    int score;
    std::vector<engine::Command*> commands;
    // Setters and Getters
  };

};

#endif
