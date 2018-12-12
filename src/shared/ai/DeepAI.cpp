#include "DeepAI.h"
#include <algorithm>
#include <iostream>
#include "AI.h"
#include "engine/AttackCommand.h"
#include "engine/DamageCommand.h"
#include "engine/MoveCommand.h"
#include "engine/MoveCommands.h"

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

DeepAI::DeepAI(State* state, Engine* engine) {
  this->state = state;
  this->engine = engine;
}

void DeepAI::buildTree(TreeNode* node, int depth) {
  if (depth == 0) {
    if (node->parent->score > scoremax) {
      nodeToRun = node->parent;
      scoremax = node->parent->score;
    }
    return;
  }
  vector<Character*> allies = state->getFight()->getFightingCharacters(1);
  vector<Character*> ennemies = state->getFight()->getFightingCharacters(0);

  vector<MoveCommands*> listmv;
  vector<AttackCommand*> listatk;

  listmv.push_back(nullptr);
  listatk.push_back(nullptr);
  for (auto character : allies) {
    for (auto pcommand : this->listCommands(character, 0)) {
      listmv.push_back(static_cast<MoveCommands*>(pcommand));
    }
    for (auto pcommand : this->listCommands(character, 1)) {
      listatk.push_back(static_cast<AttackCommand*>(pcommand));
    }
  }

  for (auto character : allies) {
    int i0 = character->getI(), j0 = character->getJ();
    for (auto mv : listmv) {
      int i, j;
      if (mv) {
        i = mv->getDiff()[0], j = mv->getDiff()[1];
      }
      for (auto atk : listatk) {
        TreeNode* childNode = new TreeNode();
        childNode->commands.push_back(mv);
        childNode->commands.push_back(atk);
        childNode->parent = node;
        Score score;
        score.setScoreAction(mv, atk, character, allies, ennemies);
        childNode->score = node->score + score.getScore();
        cout << childNode << " " << childNode->score << " " << depth << endl;
        node->children.push_back(childNode);
        if (mv) {
          MoveCommand mvcmd(state, character, i0 + i, j0 + j, i + j);
          mvcmd.execute();
        }
        if (atk) {
          DamageCommand dmgcmd(state, engine, atk->getZone(1), {-1}, 0, 0,
                               character->getWeapon()
                                   ->getAbility(atk->getAbilityNumber())
                                   ->getDamage());
          dmgcmd.execute();
        }
        buildTree(childNode, depth - 1);
        if (atk) {
          DamageCommand dmgcmd(state, engine, atk->getZone(1), {-1}, 0, 0,
                               character->getWeapon()
                                   ->getAbility(atk->getAbilityNumber())
                                   ->getDamage());
          dmgcmd.setReverse();
          dmgcmd.execute();
        }
        if (mv) {
          MoveCommand mvcmd(state, character, i0, j0, -(i + j));
          mvcmd.execute();
        }
      }
    }
  }
}

vector<Character*> DeepAI::getTurnOrder(vector<Character*> characters) {
  vector<Character*> v;
  TreeNode* root = new TreeNode();
  buildTree(root, 2);
  cout << endl << scoremax << " " << nodeToRun << endl;
  return v;
}

void DeepAI::run(Character* character) {
  // vector<Command*> commands = this->listCommands(character);
  // while (state->isFighting() && commands.size() > 0 &&
  //        character->getPvCurrent() > 0) {
  //   engine->addCommand(commands[rand() % commands.size()]);
  //   while (engine->getSize() != 0)
  //     ;
  //   commands = this->listCommands(character);
  // }
}
