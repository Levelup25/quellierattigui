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
    generator.setWorldSize({n, m});
}

void MoveCommands::setGenerator() {
    int i0 = character->getI(), j0 = character->getJ();
    int xv = (i0 / n) * n, yv = (j0 / m) * m;
    generator.clearCollisions();
    for (int l = 0; l < m; l++) {
        for (int k = 0; k < n; k++) {
            if (state->getGrid()[k + xv][l + yv]->getContent() != nothing) generator.addCollision({k, l});
        }
    }
}

void MoveCommands::addCommands(size_t i, size_t j) {
    this->setGenerator();
    int i0 = character->getI(), j0 = character->getJ();
    int xv = (i0 / n) * n, yv = (j0 / m) * m;
    generator.removeCollision({i0 - xv, j0 - yv});
    generator.removeCollision({i - xv, j - yv});
    auto path = generator.findPath({(int) (i - xv), int( j - yv)},
    {
        (int) (i0 - xv), (int) (j0 - yv)    });
    int content = (int) state->getGrid()[i][j]->getContent();
    if (content > 0 && path.size() > 0) path.pop_back();

    float step = 1.0 / 4;

    if (path.size() > 1) {
        for (auto coord = path.begin() + 1; coord != path.end(); coord++) {
            float k = (*coord).x + xv, l = (*coord).y + yv;
            if (l > j0) engine->addCommand(new DirectionCommand(character, 0));
            else if (k < i0) engine->addCommand(new DirectionCommand(character, 1));
            else if (k > i0) engine->addCommand(new DirectionCommand(character, 2));
            else if (l < j0) engine->addCommand(new DirectionCommand(character, 3));
            for (float f = 1 - step; f >= 0; f = f - step) {
                //cout << (i0 - k) * f + k << " " << (j0 - l) * f + l << endl;
                engine->addCommand(new MoveCommand(state, character, (i0 - k) * f + k, (j0 - l) * f + l));
            }
            i0 = k;
            j0 = l;
        }
        if (!state->isFighting()) {
            if (i % n == 0 && i > 0) {
                engine->addCommand(new DirectionCommand(character, 1));
                engine->addCommand(new MoveCommand(state, character, i - 1, j));
            } else if ((i + 1) % n == 0 && i < state->getI() - 1) {
                engine->addCommand(new DirectionCommand(character, 2));
                engine->addCommand(new MoveCommand(state, character, i + 1, j));
            } else if (j % m == 0 && j > 0) {
                engine->addCommand(new DirectionCommand(character, 0));
                engine->addCommand(new MoveCommand(state, character, i, j - 1));
            } else if ((j + 1) % m == 0 && j < state->getJ() - 1) {
                engine->addCommand(new DirectionCommand(character, 4));
                engine->addCommand(new MoveCommand(state, character, i, j + 1));
            }
        }
        content = (int) state->getGrid()[i][j]->getContent();

        if (content == 1) engine->addCommand(new FightCommand(state, state->getTeam(character), state->getTeam(state->getCharacter(i, j))));
    }
}