#include "Command.h"
#include <math.h>

using namespace std;
using namespace state;
using namespace engine;

void Command::setGenerator() {
    World* world = state->getWorld();
    Character* maincharacter = world->getMainCharacter();
    int i0 = maincharacter->getI(), j0 = maincharacter->getJ();
    int n = sqrt(world->getI()), m = sqrt(world->getJ());
    int xv = (i0 / n) * n, yv = (j0 / m) * m;
    generator.setWorldSize({n, m});
    for (int l = 0; l < m; l++) {
        for (int k = 0; k < n; k++) {
            if (world->getGrid()[k + xv][l + yv]->getContent() != nothing) generator.addCollision({k, l});
        }
    }
}

void Command::execute(Character* character, size_t i, size_t j) {
    World* world = state->getWorld();
    if (world->getGrid()[i][j]->getContent() == nothing) {
        int n = sqrt(world->getI()), m = sqrt(world->getJ());
        int i0 = character->getI(), j0 = character->getJ();
        int xv = (i0 / n) * n, yv = (j0 / m) * m;

        auto path = generator.findPath({i - xv, j - yv},
        {
            i0 - xv, j0 - yv
        });

        for (auto& coordinate : path) {
            world->moveCharacter(character, coordinate.x + xv, coordinate.y + yv);
        }
    }
}