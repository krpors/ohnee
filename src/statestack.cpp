#include <cassert>
#include <iostream>

#include "statestack.hpp"

StateStack::StateStack(GameState::Context context) :
context(context) {

}

void StateStack::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    assert(!this->states.empty());

    const std::unique_ptr<GameState>& state = this->states.top();
    state->draw(target, states);
}

void StateStack::update(const sf::Time& dt) {
    assert(!this->states.empty());

    std::unique_ptr<GameState>& state = this->states.top();
    state->update(dt);
}

void StateStack::handleEvent(const sf::Event& event) {
    assert(!this->states.empty());

    std::unique_ptr<GameState>& state = this->states.top();
    state->handleInput(event);

    this->applyChanges();
}

void StateStack::popState() {
    assert(!this->states.empty());

    // Note: when we pop() the state, the destructor is called automatically
    // due to it being a unique_ptr. We have to queue the popping/pushing to
    // prevent UB, that's why we push back a pending action for popping.
    this->pendingActions.push_back(1);

    std::clog << "Queueing a stack pop" << std::endl;
}

void StateStack::applyChanges() {
    if (this->pendingActions.empty()) {
        return;
    }

    std::clog << "Applying " << this->pendingActions.size() << " stack pop changes" << std::endl;
    for (auto it = this->pendingActions.begin(); it != this->pendingActions.end(); it++) {
        this->states.pop();
    }
    this->pendingActions.clear();
}

bool StateStack::isEmpty() const {
    return this->states.empty();
}
