#include "state.hpp"
#include "util.hpp"

GameState::GameState(StateStack& stack) :
stateStack(&stack) {
	debug_print("%s", "GameState()");
}

GameState::~GameState() {
	debug_print("%s", "~GameState()");
}