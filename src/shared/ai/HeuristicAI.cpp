#include "HeuristicAI.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include "engine/AttackCommand.h"
#include "engine/MoveCommands.h"

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

HeuristicAI::HeuristicAI(State* state, Engine* engine)
{
    this->state = state;
    this->engine = engine;
}

bool HeuristicAI::isCharacterAtpos(Character* c, int i, int j)
{
    vector<Character*> fighters = state->getFight()->getFightingCharacters();
    if (find(fighters.begin(), fighters.end(), c) != fighters.end())
    {
        return c->getI() == i && c->getJ() == j;
    }
    return false;
}

int HeuristicAI::getScoreAction(MoveCommands* mv,
                                AttackCommand* atk,
                                Character* character,
                                vector<Character*> allies,
                                vector<Character*> ennemies)
{
    int score = 0;
    int bonusKillEnemy = 300;
    int MalusKillAlly = 300;

    if (atk == nullptr)
        return 0;
    Weapon* weapon = character->getWeapon();
    int dmg = weapon->getAbility(atk->getAbilityNumber())->getDamage();
    auto zoneDmg = atk->getZone(1);

    for (auto coord : zoneDmg)
    {
        if (mv != nullptr)
        {
            coord[0] += mv->getDiff()[0];
            coord[1] += mv->getDiff()[1];
        }
        for (auto pcharacter : ennemies)
        {
            if (isCharacterAtpos(pcharacter, coord[0], coord[1]))
            {
                int pv = pcharacter->getPvCurrent();
                score += dmg < pv ? (dmg * 100 / pv) : bonusKillEnemy;
            }
        }
        for (auto pcharacter : allies)
        {
            if (isCharacterAtpos(pcharacter, coord[0], coord[1]))
            {
                int pv = pcharacter->getPvCurrent();
                score -= dmg < pv ? (dmg * 100 / pv) : MalusKillAlly;
            }
        }
        //int paCost = weapon.getAbility(atk->getAbilityNumber())->getPa();
        // score -= paCost;
    }
    return score;
}

std::tuple<MoveCommands*, AttackCommand*> HeuristicAI::getBestAction(
                                                                     Character* character)
{
    // get our and enemy fighters
    vector<Character*> allies = state->getFight()->getFightingCharacters(1);
    vector<Character*> ennemies = state->getFight()->getFightingCharacters(0);

    // get possible move and attack for character
    vector<MoveCommands*> listmv;
    vector<AttackCommand*> listatk;
    for (auto pcommand : this->listCommands(character, 0))
    {
        listmv.push_back(static_cast<MoveCommands*> (pcommand));
    }
    for (auto pcommand : this->listCommands(character, 1))
    {
        listatk.push_back(static_cast<AttackCommand*> (pcommand));
    }

    // complete with no move and no atk
    listmv.insert(listmv.begin(), nullptr);
    listatk.insert(listatk.begin(), nullptr);

    int scoreMax =
            getScoreAction(listmv[0], listatk[0], character, allies, ennemies);
    // search best actions
    int i = 0, j = 0;
    vector<int> imax = {0}, jmax = {0};

    // cout << "score = {";
    for (auto mv : listmv)
    {
        for (auto atk : listatk)
        {
            int score = getScoreAction(mv, atk, character, allies, ennemies);
            // cout << score << ", ";

            if (score > scoreMax)
            {
                imax.clear();
                jmax.clear();
                scoreMax = score;
            }

            if (score == scoreMax)
            {
                imax.push_back(i);
                jmax.push_back(j);
            }
            j++;
        }
        j = 0;
        i++;
    }
    // cout << "}" << endl;
    // cout << "scoreMax = " << scoreMax << endl << endl;

    // Choose one of the best action
    int r = rand() % imax.size();
    i = imax[r];
    j = jmax[r];
    /*
    cout << "score action "
         << getScoreAction(listmv[i], listatk[j], *character, allies,
                           ennemies)
         << endl
         << endl;
     */
    return std::make_tuple(listmv[i], listatk[j]);
}

void HeuristicAI::run(Character* character)
{
    while (engine->getSize() != 0);
    if (!state->isFighting())return;
    vector<Command*> listmv = this->listCommands(character, 0);
    vector<Command*> listatk = this->listCommands(character, 1);

    std::tuple<MoveCommands*, AttackCommand*> bestAction =
            this->getBestAction(character);
    MoveCommands* mv = std::get<0>(bestAction);
    //if (mv != nullptr)cout << character->getI() + mv->getDiff()[0] << " " << character->getJ() + mv->getDiff()[1] << "\t";
    AttackCommand* atk = std::get<1>(bestAction);
    //if (atk != nullptr)cout << atk->getZone(0)[0][0] << " " << atk->getZone(0)[0][1] << endl;

    if (mv != nullptr)
        engine->addCommand(mv);

    while (engine->getSize() > 0);

    //bestAction = this->getBestAction(character);
    atk = std::get<1>(bestAction);
    //if (atk != nullptr)cout << atk->getZone(0)[0][0] << " " << atk->getZone(0)[0][1] << endl << endl;

    if (atk != nullptr)
        engine->addCommand(atk);
}
