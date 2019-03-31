#include "state.hpp"
#include "util.hpp"

GameState::GameState(StateStack& stack, GameState::Context context) :
stateStack(&stack),
context(context) {
	debug_print("%s", "GameState()");
}

GameState::~GameState() {
	debug_print("%s", "~GameState()");
}