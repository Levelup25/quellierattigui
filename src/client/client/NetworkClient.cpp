#include "NetworkClient.h"

using namespace client;
using namespace engine;
using namespace std;

NetworkClient::NetworkClient(const string& url, int port, int character) {}

void NetworkClient::run() {}

string NetworkClient::getGameStatus() {}

bool NetworkClient::getServerCommands(Json::Value& out) {}

void NetworkClient::putServerCommand(Command* command) {}