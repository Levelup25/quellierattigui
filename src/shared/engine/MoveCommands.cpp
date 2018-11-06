#include "MoveCommands.h"
#include "MoveCommand.h"
#include "DirectionCommand.h"
#include "FightCommand.h"

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
    int content = (int) world->getGrid()[i][j]->getContent();
    generator.removeCollision({i0, j0});
    generator.removeCollision({i, j});
    auto path = generator.findPath({i - xv, j - yv},
    {
        i0 - xv, j0 - yv    });
    if (content > 0) {
        generator.addCollision({i, j});
        path.pop_back();
    }

    float step = 1.0 / 4;
    for (auto coord = path.begin() + 1; coord != path.end(); coord++) {
        float k = (*coord).x, l = (*coord).y;
        if (l > j0) engine->addCommand(new DirectionCommand(character, 0));
        else if (k < i0) engine->addCommand(new DirectionCommand(character, 1));
        else if (k > i0) engine->addCommand(new DirectionCommand(character, 2));
        else if (l < j0) engine->addCommand(new DirectionCommand(character, 3));
        for (float f = 1 - step; f >= 0; f = f - step) {
            //cout << (i0 - k) * f + k + xv << " " << (j0 - l) * f + l + yv << endl;
            engine->addCommand(new MoveCommand(state, character, (i0 - k) * f + k + xv, (j0 - l) * f + l + yv));
        }
        i0 = k;
        j0 = l;
    }

    if (i % n == 0 && i > 0) {
        i--;
        engine->addCommand(new DirectionCommand(character, 1));
    } else if ((i + 1) % n == 0 && i < world->getI() - 1) {
        i++;
        engine->addCommand(new DirectionCommand(character, 2));
    } else if (j % m == 0 && j > 0) {
        j--;
        engine->addCommand(new DirectionCommand(character, 0));
    } else if ((j + 1) % m == 0 && j < world->getJ() - 1) {
        j++;
        engine->addCommand(new DirectionCommand(character, 4));
    }

    content = (int) world->getGrid()[i][j]->getContent();

    if (content == 0) {
        engine->addCommand(new MoveCommand(state, character, i, j));
    }

    generator.addCollision({i, j});

    if (content > 0) {
        if (j > j0) engine->addCommand(new DirectionCommand(character, 0));
        else if (i < i0) engine->addCommand(new DirectionCommand(character, 1));
        else if (i > i0) engine->addCommand(new DirectionCommand(character, 2));
        else if (j < j0) engine->addCommand(new DirectionCommand(character, 3));
    }

    if (content == 1) engine->addCommand(new FightCommand(state, world->getTeam(character), world->getTeam(world->getCharacter(i, j))));
}