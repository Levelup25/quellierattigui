#include "HeuristicAI.h"
#include "engine/MoveCommands.h"
#include "engine/AttackCommand.h"
#include <iostream>

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

HeuristicAI::HeuristicAI(State* state, Engine* engine) {
    this->state = state;
    this->engine = engine;
}

vector<vector<int>> HeuristicAI::getScore(Character* character) {
    vector<Command*> listmv = this->listCommands(character, 0);
    vector<Command*> listatk = this->listCommands(character, 1);
    vector<vector<int>> score(listmv.size(), vector<int> (listatk.size(), 0));
    vector<Character*> chars0 = state->getFight()->getFightingCharacters(0);
    vector<Character*> chars1 = state->getFight()->getFightingCharacters(1);
    Weapon* w = character->getWeapon();
    int i = 0, j = 0, scoremax = 0;
    vector<int> imax, jmax;
    for (auto mv : listmv) {
        for (auto atk : listatk) {
            int dmg = w->getAbility(static_cast<AttackCommand*> (atk)->getAbilityNumber())->getDamage();
            vector<vector<int>> zone = static_cast<AttackCommand*> (atk)->getZone(1, false);
            //score[i][j] -= (abs(static_cast<MoveCommands*> (mv)->getDiff()[0]) + abs(static_cast<MoveCommands*> (mv)->getDiff()[1]) + w->getAbility(static_cast<AttackCommand*> (atk)->getAbilityNumber())->getPa());
            for (auto coord : zone) {
                coord[0] += static_cast<MoveCommands*> (mv)->getDiff()[0];
                coord[1] += static_cast<MoveCommands*> (mv)->getDiff()[1];
                for (c0 : chars0) {
                    if (c0->getI() == coord[0] && c0->getJ() == coord[1]) {
                        score[i][j] += dmg;
                        if (c0->getPvCurrent() <= dmg)score[i][j] += 1;
                    }
                }
                for (c1 : chars1) {
                    if (c1->getI() == coord[0] && c1->getJ() == coord[1]) {
                        score[i][j] -= dmg;
                        if (c1->getPvCurrent() <= dmg)score[i][j] -= 1;
                    }
                }
            }
            if (scoremax <= score[i][j]) {
                if (scoremax < score[i][j]) {
                    scoremax = score[i][j];
                    imax.clear();
                    jmax.clear();
                }
                imax.push_back(i);
                jmax.push_back(j);
            }
            //cout << score[i][j] << " ";
            j++;
        }
        //cout << endl;
        i++;
        j = 0;
    }
    //cout << endl;
    return
    {
        {
            scoremax
        }, imax, jmax
    };
}

void HeuristicAI::run(Character* character) {
    while (engine->getSize() != 0);
    vector<Command*> listmv = this->listCommands(character, 0);
    vector<Command*> listatk = this->listCommands(character, 1);
    vector<vector<int>> score = this->getScore(character);
    int scoremax = score[0][0];
    if (scoremax > 0) {
        cout << scoremax << endl;
        vector<int> imax = score[1], jmax = score[2];
        int r = rand() % imax.size();
        engine->addCommand(listmv[imax[r]]);
        while (engine->getSize() != 0);
        listatk = this->listCommands(character, 1);
        engine->addCommand(listatk[jmax[r]]);
        while (engine->getSize() != 0);
        listmv = this->listCommands(character, 0);
        listatk = this->listCommands(character, 1);
        vector<vector<int>> score = this->getScore(character);
        int scoremax = score[0][0];
    } else engine->addCommand(listmv[rand() % listmv.size()]);
}
