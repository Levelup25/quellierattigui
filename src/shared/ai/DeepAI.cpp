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
  if (depth == 0)
    return;
  // get our and enemy fighters
  vector<Character*> allies = state->getFight()->getFightingCharacters(1);
  vector<Character*> ennemies = state->getFight()->getFightingCharacters(0);

  // get possible moves and attacks for the character
  vector<MoveCommands*> listmv;
  vector<AttackCommand*> listatk;
  // complete with no move and no atk
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
        childNode->score.setScoreAction(mv, atk, character, allies, ennemies);
        node->children.push_back(childNode);
        cout << childNode->score << endl;
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
        if (mv) {
          MoveCommand mvcmd(state, character, i0, j0, -(i + j));
          mvcmd.execute();
        }
        if (atk) {
          DamageCommand dmgcmd(state, engine, atk->getZone(1), {-1}, 0, 0,
                               -character->getWeapon()
                                    ->getAbility(atk->getAbilityNumber())
                                    ->getDamage());
          dmgcmd.execute();
        }
      }
    }
  }
}

vector<Character*> DeepAI::getTurnOrder(vector<Character*> characters) {
  vector<Character*> v;
  vector<int> dist;
  for (auto c : characters) {
    int d = 0;
    for (auto c2 : state->getFight()->getFightingCharacters(0))
      d += abs(c->getI() - c2->getI()) + abs(c->getJ() - c2->getJ());
    dist.push_back(d);
  }

  int max =
      dist[distance(dist.begin(), max_element(dist.begin(), dist.end()))] + 1;
  for (int j = 0; j < (int)characters.size(); j++) {
    int i = distance(dist.begin(), min_element(dist.begin(), dist.end()));
    v.push_back(characters[i]);
    dist[i] = max;
  }
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
  TreeNode* root = new TreeNode();
  buildTree(root, 3);
}
