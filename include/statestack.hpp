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
 * This class encapsulates the states of the game, backed by a vector. The class
 * is made sf::Drawable so we can easily draw things.
 */
class StateStack : public sf::Drawable {
public:
    StateStack(GameState::Context context);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update(const sf::Time& dt);
    void handleEvent(const sf::Event& event);

    template<typename T>
    void pushState();

    void popState();

    bool isEmpty() const;

    /**
     * Registers a particular state with the given ID.
     */
    template<typename T>
    void registerState(StateId id);

private:
    GameState::Context context;

    std::stack<std::unique_ptr<GameState>> states;

    std::map<StateId, std::unique_ptr<GameState>> stateMap;
};

// Note to self: the best practice for templates seem to be to define them in
// the header files, since the implementation is created at compile time. See
// https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl for more
// information.
template<typename T>
void StateStack::pushState() {
    this->states.push(std::unique_ptr<T>(new T(*this, context)));
}


// TODO: register states based on state id (enum). This fixes a crapload of
// circular dependencies.

#endif // STATESTACK_HPP