#include <cassert>
#include <iostream>

#include "statestack.hpp"

// BUG: we MUST queue the popping actions because popState() will effectively
// call the deleter on the state. This will result in undefined behaviour.

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
}

void StateStack::popState() {
    assert(!this->states.empty());

    // Note: when we pop() the state, the destructor is called automatically
    // due to it being a unique_ptr. We have to queue the popping/pushing to
    // prevent UB.
    this->states.pop();
}

bool StateStack::isEmpty() const {
    return this->states.empty();
}
