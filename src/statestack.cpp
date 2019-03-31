#include <cassert>

#include "statestack.hpp"

StateStack::StateStack(GameState::Context context) :
        context(context) {
    // TODO: pass some context object in.
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

    this->states.pop();
}

bool StateStack::isEmpty() const {
    return this->states.empty();
}