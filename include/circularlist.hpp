#ifndef CIRCULAR_LIST_HPP
#define CIRCULAR_LIST_HPP

/**
 * This rather simple template is a circular list, backed by a vector.
 * Each time the next() method is called, it's checked whether the end is
 * eached. If so, the iterator jumps back to the start. This template is used
 * for instance to play the same sound multiple times in a row (e.g. menu
 * selection, or death sounds).
 */
template<typename T>
class CircularList {
public:
	CircularList() {
		this->it = this->vec.begin();
	}

	void push_back(T& t) {
		this->vec.push_back(t);
		this->it = this->vec.begin();
	}

	void push_back(T&& t) {
		this->vec.push_back(t);
		this->it = this->vec.begin();
	}

	void clear() {
		this->vec.clear();
		this->it = this->vec.begin();
	}

	/**
	 * Returns the begin iterator of the backing vector.
	 */
	typename std::vector<T>::iterator begin() {
		return vec.begin();
	}

	/**
	 * Return the end iterator of the backing vector.
	 */
	typename std::vector<T>::iterator end() {
		return vec.end();
	}

	/**
	 * Delegate to the vector's emplace_back using an std::forward.
	 */
	template<typename... Args>
	void emplace_back(Args&&... args) {
		this->vec.emplace_back(std::forward<Args>(args)...);
		this->it = this->vec.begin();
	}

	/**
	 * Returns the next element in a cyclic iterating fashion. This means
	 * that if the end is reached, the iterator will rotate back to the beginning
	 * of the vector.
	 */
	T& next() {
		// Get the value at the position where the iterator currently is.
		T& ret = *this->it;

		// Advance the itartor, and check whether we hit the end. If so,
		// assign the iterator to the beginning.
		this->it++;
		if (this->it == this->vec.end()) {
			this->it = this->vec.begin();
		}

		// Return the ref.
		return ret;
	}
private:
	std::vector<T> vec;
	typename std::vector<T>::iterator it;
};


#endif // CIRCULAR_LIST_HPP
