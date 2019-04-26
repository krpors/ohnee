#include <cassert>
#include <iostream>

#include "statestack.hpp"
#include "util.hpp"

static inline const std::string stateIdToString(StateId id) {
	switch (id) {
	case StateId::None:  return "NoneState";
	case StateId::Intro: return "Intro";
	case StateId::Menu:  return "MenuState";
	case StateId::Game:  return "GameState";
	case StateId::Pause: return "PauseState";
	default: return "!UNHANDLED STATE ID!";
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
	// TODO: build will fail without NDEBUG because 'it' is not used
	auto it = this->stateMap.find(id);
	assert (it != this->stateMap.end());

	TRACE("Pushing state '" << stateIdToString(id) << "'");
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
	// When  there are no pending actions, return immediately.
	if (this->pendingActions.empty()) {
		return;
	}

	for (const auto& a : this->pendingActions) {
		if (a.action == PendingAction::ActionType::Push) {
			auto it = this->stateMap.find(a.id);
			assert (it != this->stateMap.end());

			// first, deactivate the current state, when a new state will be
			// pushed. This allows us to pause music or sound effects etc, which
			// are local to a particular state of the game.
			if (!this->states.empty()) {
				this->states.top()->deactivate();
			}

			this->states.push(it->second());
		} else if (a.action == PendingAction::ActionType::Pop) {
			this->states.pop();
		}
	}

	// when we're done, activate the top.
	if (!this->states.empty()) {
		this->states.top()->activate();
	}

	this->pendingActions.clear();
}

bool StateStack::isEmpty() const {
	return this->states.empty();
}
