#include "AttackCommand.h"
#include "AnimationCommand.h"
#include "FightCommand.h"
#include <iostream>

using namespace std;
using namespace state;
using namespace engine;

AttackCommand::AttackCommand(State* state, Engine* engine, Character* character, vector<int> position, int abilityNumber) {
    this->state = state;
    this->engine = engine;
    this->character = character;
    this->abilityNumber = abilityNumber;
    this->position = position;
}

void AttackCommand::setZones() {
    int n = state->getN(), m = state->getM();
    ability = character->getWeapon()->getAbility(abilityNumber);
    if (ability->getPa() <= character->getPa()) {
        int i0 = character->getI(), j0 = character->getJ();
        int i = position[0], j = position[1];
        targets = ability->getTargetZone({i0, j0});
        for (auto target = targets.end() - 1; target >= targets.begin(); target--) {
            if ((*target)[0] / n != i0 / n || (*target)[0] < 0 || (*target)[1] / m != j0 / m || (*target)[1] < 0) targets.erase(target);
        }
        for (auto coord : targets) {
            if (coord[0] == i && coord[1] == j) {
                effects = ability->getEffectZone({i, j});
                for (auto effect = effects.end() - 1; effect >= effects.begin(); effect--) {
                    if ((*effect)[0] / n != i0 / n || (*effect)[0] < 0 || (*effect)[1] / m != j0 / m || (*effect)[1] < 0) effects.erase(effect);
                }
                return;
            }
        }
        effects = {};
    } else {
        targets = {};
        effects = {};
    }
}

vector<vector<int>> AttackCommand::getZone(size_t i) {
    if (i == 0)return targets;
    else if (i == 1) return effects;
    return
    {
    };
}

void AttackCommand::execute() {
    this->setZones();
    if (effects.size() > 0 && ability->getPa() <= character->getPa()) {
        character->removePa(ability->getPa());

        float pas = 12;
        float I = (position[0] - character->getI()) / pas, J = (position[1] - character->getJ()) / pas;
        int direction;
        if (I > 0 && J == 0) direction = 0;
        else if (I == 0 && J < 0) direction = 1;
        else if (I < 0 && J == 0) direction = 2;
        else if (I == 0 && J > 0) direction = 3;
        else if (I > 0 && J > 0) direction = 4;
        else if (I < 0 && J > 0) direction = 5;
        else if (I < 0 && J < 0) direction = 6;
        else direction = 7; //if (I > 0 && J < 0)

        vector<vector<int>> v;
        vector<int> v2{
            direction
        };

        for (int i = 0; i <= pas; i++) {
            v = {
                {
                    character->getI() + i*I, character->getJ() + i * J
                }
            };
            engine->addCommand(new AnimationCommand(state, v, v2, ability->getElement(), ability->getLv()));
        }

        int dist = 1;
        for (auto effect : effects) {
            if (effect != position) {
                if (abs(effect[0] - position[0]) + abs(effect[1] - position[1]) > dist) {
                    dist = abs(effect[0] - position[0]) + abs(effect[1] - position[1]);
                    engine->addCommand(new AnimationCommand(state, v, v2, ability->getElement(), ability->getLv()));
                }
                float I = (effect[0] - position[0]) / pas, J = (effect[1] - position[1]) / pas;
                if (I > 0 && J == 0) direction = 0;
                else if (I == 0 && J < 0) direction = 1;
                else if (I < 0 && J == 0) direction = 2;
                else if (I == 0 && J > 0) direction = 3;
                else if (I > 0 && J > 0) direction = 4;
                else if (I < 0 && J > 0) direction = 5;
                else if (I < 0 && J < 0) direction = 6;
                else direction = 7;
                v.push_back(effect);
                v2.push_back(direction);
            }
            Character* c = state->getCharacter(effect[0], effect[1]);
            if (c != nullptr) {
                c->removePv(ability->getDamage());
                if (c->getPv() <= 0) state->getCell(c->getI(), c->getJ())->setContent(nothing);
                //                    Team* t = state->getTeam(c);
                //                    if (t->getCharacters().size() > 1) {
                //                        if (c != t->getMainCharacter()) t->swapCharacters(c, t->getMainCharacter());
                //                        else t->swapCharacters(0, 1);
                //                    }
                // state->delCharacter(c);
                //}
            }
        }
        for (int i = 0; i < 25; i++)engine->addCommand(new AnimationCommand(state, v, v2, ability->getElement(), ability->getLv()));
        v.clear();
        engine->addCommand(new AnimationCommand(state, v, v2, ability->getElement(), ability->getLv()));
        state->endFight();
        if (!state->isFighting()) state->etatCombat = 0;
        //engine->addCommand(new FightCommand(state, nullptr, nullptr));
    }
}