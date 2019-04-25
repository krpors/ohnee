#ifndef STATESTACK_HPP
#define STATESTACK_HPP

#include <functional>
#include <map>
#include <memory>
#include <stack>

#include <SFML/Graphics.hpp>

#include "state.hpp"
#include "stateids.hpp"


class GameState;

/**
 * This class encapsulates the states of the game, backed by a stack. The class
 * is made sf::Drawable so we can easily draw things.
 */
class StateStack : public sf::Drawable {
public:
	StateStack(GameState::Context context);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(const sf::Time& dt);
	void handleEvent(const sf::Event& event);

	/**
	 * Directly pushes a state to the stack. This is not a queued action.
	 */
	void pushState(StateId id);

	/**
	 * Queues a state pop from the stack.
	 */
	void popState();

	/**
	 * Applies the pending pop actions on the stackstate. The actions are read from
	 * the pendingActions vector.
	 */
	void applyChanges();

	bool isEmpty() const;

	/**
	 * Registers a particular state with the given ID.
	 */
	template<typename T>
	void registerState(StateId id);

private:

	/**
	 * An action which can be executed on the stack state must be "queued".
	 * This action can be added to the pendingActions vector and must be executed
	 * at the end of the game loop.
	 */
	class PendingAction {
	public:
		/**
		 * A pendingaction can have a type. This enumeration defines that.
		 */
		enum ActionType {
			Push,
			Pop,
			Clear
		};

		PendingAction(ActionType a, StateId id = StateId::None) : action(a), id(id) {}
		ActionType action;
		StateId id;
	};

	GameState::Context context;

	/**
	 * Stack with the current states.
	 */
	std::stack<std::unique_ptr<GameState>> states;

	/**
	 * This map contains a stateid mapped to a factory function.
	 */
	std::map<StateId, std::function<std::unique_ptr<GameState>()>> stateMap;

	/**
	 * A vector with the pending actions to run after each game loop. This is
	 * because a GameState has a relationship with the StateStack. When we pop
	 * a GameState from the stack, it is effectively destroyed (because the
	 * unique_ptr goes out of scope). Because of this, we are not allowed to
	 * execute gameState->stateStack->pop() twice, because the second call would
	 * execute the pop on a stateStack on the destroyed gameState!
	 *
	 * This idea was stolen a bit from the SFML book, chapter 5 (States).
	 */
	std::vector<PendingAction> pendingActions;
};

// Note to self: the best practice for templates seem to be to define them in
// the header files, since the implementation is created at compile time. See
// https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl for more
// information.
template<typename T>
void StateStack::registerState(StateId id) {
	auto yo = [this]() {
		return std::unique_ptr<T>(new T(*this, context));
	};

	this->stateMap[id] = yo;
}

#endif // STATESTACK_HPP
