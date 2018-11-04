#include "MoveCommands.h"
#include "MoveCommand.h"
#include "FightCommand.h"
#include <iostream>

using namespace std;
using namespace state;
using namespace engine;

MoveCommands::MoveCommands(State* state, Engine* engine, Character* character) {
    this->state = state;
    this->character = character;
    this->engine = engine;
    setGenerator();
}

void MoveCommands::setGenerator() {
    World* world = state->getWorld();
    int i0 = character->getI(), j0 = character->getJ();
    int xv = (i0 / n) * n, yv = (j0 / m) * m;
    generator.setWorldSize({n, m});
    for (int l = 0; l < m; l++) {
        for (int k = 0; k < n; k++) {
            if (world->getGrid()[k + xv][l + yv]->getContent() != nothing) generator.addCollision({k, l});
        }
    }
}

void MoveCommands::addCommands(size_t i, size_t j) {
    World* world = state->getWorld();
    int i0 = character->getI(), j0 = character->getJ();
    int xv = (i0 / n) * n, yv = (j0 / m) * m;
    generator.removeCollision({i0, j0});
    generator.removeCollision({i, j});
    auto path = generator.findPath({i - xv, j - yv},
    {
        i0 - xv, j0 - yv    });
    if ((int) world->getGrid()[i][j]->getContent() > 0) {
        generator.addCollision({i, j});
        path.pop_back();
    }

    float step = 1.0 / 4;
    for (auto coord = path.begin() + 1; coord != path.end(); coord++) {
        float i = (*coord).x, j = (*coord).y;
        for (float f = 1 - step; f >= 0; f = f - step) {
            //cout << (i0 - i) * f + i + xv << " " << (j0 - j) * f + j + yv << endl;
            engine->addCommand(new MoveCommand(state, character, (i0 - i) * f + i + xv, (j0 - j) * f + j + yv));
        }
        i0 = i;
        j0 = j;
    }
    generator.addCollision({i, j});
    if ((int) world->getGrid()[i][j]->getContent() == 1) engine->addCommand(new FightCommand(state, world->getTeam(character), world->getTeam(world->getCharacter(i, j))));
}