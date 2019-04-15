#include <cassert>
#include <iostream>

#include "statestack.hpp"

static inline const std::string stateIdToString(StateId id) {
    switch (id) {
    case StateId::None:  return "NoneState";
    case StateId::Intro: return "Intro";
    case StateId::Menu:  return "MenuState";
    case StateId::Game:  return "GameState";
    case StateId::Pause: return "PauseState";
    default: return "";
    }
}

// =============================================================================

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

    std::unique_ptr<GameState>& state = this->states.top();
    state->handleInput(event);

    this->applyChanges();
}

void StateStack::pushState(StateId id) {
    auto it = this->stateMap.find(id);
    assert (it != this->stateMap.end());

    std::cout << "Pushing state " << stateIdToString(id) << std::endl;
    this->pendingActions.emplace_back(PendingAction::ActionType::Push, id);
}

void StateStack::popState() {
    assert(!this->states.empty());

    // Note: when we pop() the state, the destructor is called automatically
    // due to it being a unique_ptr. We have to queue the popping/pushing to
    // prevent UB, that's why we push back a pending action for popping.
    this->pendingActions.emplace_back(PendingAction::ActionType::Pop);
}

void StateStack::applyChanges() {
    for (const auto& a : this->pendingActions) {
        if (a.action == PendingAction::ActionType::Push) {
            auto it = this->stateMap.find(a.id);
            assert (it != this->stateMap.end());
            this->states.push(it->second());
        } else if (a.action == PendingAction::ActionType::Pop) {
            this->states.pop();
        }
    }

    this->pendingActions.clear();
}

bool StateStack::isEmpty() const {
    return this->states.empty();
}
