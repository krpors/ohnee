#include <functional>
#include <map>
#include <memory>
#include <stack>

#include <SFML/Graphics.hpp>

#include "state.hpp"
#include "stateids.hpp"

#ifndef STATESTACK_HPP
#define STATESTACK_HPP

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
    enum Action {
        Push,
        Pop,
        Clear
    };

    class PendingAction {
    public:
        PendingAction(Action a, StateId id = StateId::None) : action(a), id(id) {}
        Action action;
        StateId id;
    };

    GameState::Context context;

    std::stack<std::unique_ptr<GameState>> states;

    std::map<StateId, std::function<std::unique_ptr<GameState>()>> stateMap;

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

// TODO: register states based on state id (enum). This fixes a crapload of
// circular dependencies.

#endif // STATESTACK_HPP
