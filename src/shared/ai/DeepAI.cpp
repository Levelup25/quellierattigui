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

vector<tuple<MoveCommands*, AttackCommand*, Score>> DeepAI::getBestActions(
    Character* character,
    int threshold) {
  vector<tuple<MoveCommands*, AttackCommand*, Score>> actions;
  // get our and enemy fighters
  shared_ptr<Fight> fight = state->getFight();
  vector<Character*> allies, ennemies;
  if (fight->getTeam(1)->getCharacter(character)) {
    allies = fight->getFightingCharacters(1);
    ennemies = fight->getFightingCharacters(0);
  } else {  // if (fight->getTeam(0)->getCharacter(character)) {
    allies = fight->getFightingCharacters(0);
    ennemies = fight->getFightingCharacters(1);
  }

  // get possible moves and attacks for the character
  vector<MoveCommands*> listmv;
  vector<AttackCommand*> listatk;
  // complete with no move and no atk
  listmv.push_back(nullptr);
  listatk.push_back(nullptr);

  for (auto pcommand : listCommands(character, 0)) {
    listmv.push_back(static_cast<MoveCommands*>(pcommand));
  }
  for (auto pcommand : listCommands(character, 1)) {
    listatk.push_back(static_cast<AttackCommand*>(pcommand));
  }

  Score score;
  score.setScoreAction(listmv[0], listatk[0], character, allies, ennemies);
  vector<Score> scoresBest;
  int scoreMax = score.getScore();
  // search best actions
  int i = 0, j = 0;
  vector<int> imax = {0}, jmax = {0};

  for (auto mv : listmv) {
    for (auto atk : listatk) {
      Score score;
      score.setScoreAction(mv, atk, character, allies, ennemies);

      if (threshold < 0) {
        if (score.getScore() > scoreMax) {
          imax.clear();
          jmax.clear();
          scoresBest.clear();
          scoreMax = score.getScore();
        }

        if (score.getScore() == scoreMax) {
          imax.push_back(i);
          jmax.push_back(j);
          scoresBest.push_back(score);
        }
      } else {
        if (score.getScore() >= threshold) {
          imax.push_back(i);
          jmax.push_back(j);
          scoresBest.push_back(score);
        }
      }
      j++;
    }
    j = 0;
    i++;
  }

  // Return all good actions
  for (int r = 0; r < (int)scoresBest.size(); r++) {
    i = imax[r];
    j = jmax[r];
    actions.push_back(make_tuple(listmv[i], listatk[j], scoresBest[r]));
  }
  return actions;
}

void DeepAI::buildTree(shared_ptr<TreeNode> node, int depth, int teamNumber) {
  int threshold = 200;
  if (depth == 0) {
    if (node->score > scoremax) {
      nodeToRun = node;
      scoremax = node->score;
    }
    return;
  }

  shared_ptr<Fight> fight = state->getFight();
  vector<Character*> allies, ennemies;

  allies = fight->getFightingCharacters(teamNumber);
  ennemies = fight->getFightingCharacters(1 - teamNumber);

  vector<vector<tuple<MoveCommands*, AttackCommand*, Score>>> actionss;
  for (auto character : allies) {
    actionss.push_back(getBestActions(character, threshold));
  }

  if (actionss.size() == 0) {
    // depth--;
    // if (depth == 0) {
    //   if (node->parent->score > scoremax) {
    //     nodeToRun = node->parent;
    //     scoremax = node->parent->score;
    //   }
    //   return;
    // }
    teamNumber = 1 - teamNumber;
    allies = fight->getFightingCharacters(teamNumber);
    ennemies = fight->getFightingCharacters(1 - teamNumber);
    for (auto character : allies) {
      actionss.push_back(getBestActions(character, threshold));
    }
  }

  int k = 0, k2 = 0;
  for (auto character : allies) {
    if (character == node->character)
      break;
    k2++;
  }
  for (auto character : allies) {
    if (node->character == nullptr || node->character == character ||
        actionss[k2].size() == 0) {
      auto actions = actionss[k++];
      int i0 = character->getI(), j0 = character->getJ();
      for (auto action : actions) {
        MoveCommands* mv = get<0>(action);
        AttackCommand* atk = get<1>(action);
        int i, j;
        if (mv) {
          i = mv->getDiff()[0], j = mv->getDiff()[1];
        }
        shared_ptr<TreeNode> childNode(new TreeNode());
        childNode->commands.push_back(mv);
        childNode->commands.push_back(atk);
        childNode->character = character;
        childNode->parent = node;
        childNode->teamNumber = teamNumber;
        childNode->score =
            node->score + (2 * teamNumber - 1) * get<2>(action).getScore();
        // cout << childNode << " " << teamNumber << " " <<
        // get<2>(action).getScore()
        //      << " " << depth << endl;
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
  scoremax = 0;
  shared_ptr<TreeNode> root(new TreeNode());
  buildTree(root, 2);
  // cout << endl << scoremax << " " << nodeToRun << endl;
  shared_ptr<TreeNode> node = nodeToRun;
  if (node) {
    vector<Command*> cmds;
    while (node != root) {
      cmds.push_back(node->commands[1]);
      cmds.push_back(node->commands[0]);
      node = node->parent;
    }
    while (cmds.size()) {
      if (cmds.back())
        engine->addCommand(cmds.back());
      cmds.pop_back();
      while (engine->getSize())
        ;
    }
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
}
