#include <cassert>
#include <iostream>

#include "statestack.hpp"

StateStack::StateStack(GameState::Context context) :
context(context) {

}

void StateStack::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (this->states.empty()) {
        return;
    }

    const std::unique_ptr<GameState>& state = this->states.top();
    state->draw(target, states);
}

void StateStack::update(const sf::Time& dt) {
    if (this->states.empty()) {
        return;
    }

    std::unique_ptr<GameState>& state = this->states.top();
    state->update(dt);
}

void StateStack::handleEvent(const sf::Event& event) {
    if (this->states.empty()) {
        std::cout << "State stack is empty, returning from handleEvent" << std::endl;
        return;
    }

    std::cout << "what" << std::endl;

    std::unique_ptr<GameState>& state = this->states.top();
    state->handleInput(event);

    this->applyChanges();
}

void StateStack::pushState(StateId id) {
    auto it = this->stateMap.find(id);
    assert (it != this->stateMap.end());

    std::cout << "Pushing state " << id << std::endl;
    this->pendingActions.emplace_back(Action::Push, id);
}

void StateStack::popState() {
    assert(!this->states.empty());

    // Note: when we pop() the state, the destructor is called automatically
    // due to it being a unique_ptr. We have to queue the popping/pushing to
    // prevent UB, that's why we push back a pending action for popping.
    this->pendingActions.emplace_back(Action::Pop);

    std::clog << "Queueing a stack pop" << std::endl;
}

void StateStack::applyChanges() {
    std::cout << "yes" << std::endl;
    for (auto it = this->pendingActions.begin(); it != this->pendingActions.end(); it++) {
        std::cout << "BOASDOADA" << std::endl;
        const PendingAction& a = *it;
        if (a.action == Action::Push) {
            auto it = this->stateMap.find(a.id);
            assert (it != this->stateMap.end());
            this->states.push(it->second());
        } else if (a.action == Action::Pop) {
            this->states.pop();
        }
    }

    this->pendingActions.clear();
}

bool StateStack::isEmpty() const {
    return this->states.empty();
}
