#include "AttackCommand.h"
#include <iostream>

using namespace std;
using namespace state;
using namespace engine;

AttackCommand::AttackCommand(State* state, Character* character, vector<int> position, int abilityNumber) {
    this->state = state;
    this->character = character;
    this->abilityNumber = abilityNumber;
    this->position = position;
}

void AttackCommand::setZones() {
    ability = character->getWeapon()->getAbility(abilityNumber);
    if (ability->getPa() <= character->getPa()) {
        int i0 = character->getI(), j0 = character->getJ();
        int i = position[0], j = position[1];
        targets = ability->getTargetZone({i0, j0});
        for (auto coord : targets) {
            if (coord[0] == i && coord[1] == j) {
                effects = ability->getEffectZone({i, j});
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
        for (auto effect : effects) {
            Character* c = state->getCharacter(effect[0], effect[1]);
            if (c != nullptr) {
                c->removePv(ability->getDamage());
                if (c->getPv() == 0) {
                    state->getCell(position[0], position[1])->setContent(nothing);
                    state->delCharacter(c);
                }
            }
        }
        state->endFight();
        if (!state->isFighting()) state->etatCombat = 0;
    }
}