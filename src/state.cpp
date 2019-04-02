#include "state.hpp"
#include "util.hpp"

GameState::GameState(StateStack& stack, GameState::Context context) :
stateStack(&stack),
context(context) {
}

GameState::~GameState() {
}
