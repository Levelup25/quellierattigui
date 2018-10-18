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

//std::vector<Cell&> Ability::getPossibleTarget() {
//
//}
//
//std::vector<Cell&> Ability::getEffectZone(Cell& target) {
//
//}

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