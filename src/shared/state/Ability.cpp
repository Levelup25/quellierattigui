#include "Ability.h"

using namespace std;
using namespace state;

Ability::Ability(string name, int pa, int damage, ElementType element, int cooldown, ZoneType targetType, int targetMin, int targetMax, ZoneType effectType, int effectMin, int effectMax) {
    cooldownInitial = cooldown;
    damageReduce = 1;
}
