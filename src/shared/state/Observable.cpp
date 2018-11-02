#include "Observable.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;
using namespace state;

void Observable::registerObserver(Observer* o) {
    observers.push_back(o);
}

void Observable::unregisterObserver(Observer* o) {
    for (auto obs = observers.begin(); obs != observers.end(); ++obs) {
        if ((*obs) == o) observers.erase(obs);
    }
}

void Observable::unregisterObservers() {
    observers.clear();
}

void Observable::notifyObservers(Event* e) {
    for (auto obs = observers.begin(); obs != observers.end(); ++obs) {
        (*obs)->stateChanged(e);
    }
}