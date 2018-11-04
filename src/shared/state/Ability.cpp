#include "Ability.h"

using namespace std;
using namespace state;

Ability::Ability(std::string name, unsigned int pa, int damage, ElementType element, int damageReduce, int cooldown, ZoneType targetType, int targetMin, int targetMax, ZoneType effectType, int effectMin, int effectMax) {
    this->name = name;
    this->pa = pa;
    this->damage = damage;
    this->element = element;
    this->damageReduce = damageReduce;
    this->cooldownInitial = cooldown;
    this->targetType = targetType;
    this->targetMin = targetMin;
    this->targetMax = targetMax;
    this->effectType = effectType;
    this->effectMin = effectMin;
    this->effectMax = effectMax;
}

vector<vector<int>> Ability::getTargetZone(vector<int> position) {
    vector<vector<int>> targets;
    int i = position[0], j = position[1];
    for (int k = targetMin; k <= targetMax; k++) {
        if (k == 0) targets.push_back({i, j});
        else {
            if (targetType == circle) {
                for (int l = 0; l < k; l++) {
                    targets.push_back({i - l, j - k + l});
                    targets.push_back({i - k + l, j + l});
                    targets.push_back({i + k - l, j - l});
                    targets.push_back({i + l, j + k - l});
                }
            } else if (targetType == line) {
                targets.push_back({i, j - k});
                targets.push_back({i - k, j});
                targets.push_back({i + k, j});
                targets.push_back({i, j + k});

            } else if (targetType == diag) {
                targets.push_back({i - k, j - k});
                targets.push_back({i - k, j + k});
                targets.push_back({i + k, j - k});
                targets.push_back({i + k, j + k});
            }
        }
    }
    return targets;
}

vector<vector<int>> Ability::getEffectZone(vector<int> target) {
    vector<vector<int>> effects;
    int i = target[0], j = target[1];
    for (int k = effectMin; k <= effectMax; k++) {
        if (k == 0) effects.push_back({i, j});
        else {
            if (effectType == circle) {
                for (int l = 0; l < k; l++) {
                    effects.push_back({i - l, j - k + l});
                    effects.push_back({i - k + l, j + l});
                    effects.push_back({i + k - l, j - l});
                    effects.push_back({i + l, j + k - l});
                }
            } else if (effectType == line) {
                effects.push_back({i, j - k});
                effects.push_back({i - k, j});
                effects.push_back({i + k, j});
                effects.push_back({i, j + k});

            } else if (effectType == diag) {
                effects.push_back({i - k, j - k});
                effects.push_back({i - k, j + k});
                effects.push_back({i + k, j - k});
                effects.push_back({i + k, j + k});
            }
        }
    }
    return effects;
}

void Ability::setCooldown(int initial) {
    cooldownInitial = initial;
}

void Ability::setTarget(ZoneType targetZone, int min, int max) {
    targetType = targetZone;
    targetMin = min;
    targetMax = max;
}

void Ability::setEffect(ZoneType effectZone, int min, int max, int reduce) {
    effectType = effectZone;
    effectMin = min;
    effectMax = max;
    damageReduce = reduce;
}

int Ability::getDamage() {
    return damage;
}

unsigned int Ability::getPa() {
    return pa;
}

string Ability::getName() {
    return name;
}