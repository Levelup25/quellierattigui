#include "Ability.h"

namespace state {
Ability::Ability(int pa,
                 int damage,
                 ElementType element,
                 int cooldown,
                 ZoneType targetType,
                 int targetMin,
                 int targetMax,
                 ZoneType effectType,
                 int effectMin,
                 int effectMax) {
  cooldownCurrent = cooldown;
  damageReduce = 0;
}

}  // namespace state
