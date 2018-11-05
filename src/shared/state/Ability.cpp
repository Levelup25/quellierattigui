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

vector<vector<int>> Ability::getZone(vector<int> position, ZoneType zone, int min, int max) {
    vector<vector<int>> zones;
    int i = position[0], j = position[1];
    for (int k = min; k <= max; k++) {
        if (k == 0) zones.push_back({i, j});
        else {
            if (zone == circle) {
                for (int l = 0; l < k; l++) {
                    zones.push_back({i - l, j - k + l});
                    zones.push_back({i - k + l, j + l});
                    zones.push_back({i + k - l, j - l});
                    zones.push_back({i + l, j + k - l});
                }
            } else if (zone == line) {
                zones.push_back({i, j - k});
                zones.push_back({i - k, j});
                zones.push_back({i + k, j});
                zones.push_back({i, j + k});

            } else if (zone == diag) {
                zones.push_back({i - k, j - k});
                zones.push_back({i - k, j + k});
                zones.push_back({i + k, j - k});
                zones.push_back({i + k, j + k});
            }
        }
    }
    return zones;
}

vector<vector<int>> Ability::getTargetZone(vector<int> position) {
    return getZone(position, targetType, targetMin, targetMax);
}

vector<vector<int>> Ability::getEffectZone(vector<int> position) {
    return getZone(position, effectType, effectMin, effectMax);
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