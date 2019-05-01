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

	void push_back(T& t) {
		this->vec.push_back(t);
	}

	void push_back(T&& t) {
		this->vec.push_back(t);
	}

	void clear() {
		this->vec.clear();
		this->i = 0;
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
	}

	/**
	 * Recede to the previous element in the backing vector, and return it. If
	 * the beginning is reached, it will 'cycle' to the end of the vector instead.
	 */
	T& prev() {
		this->i--;
		if (this->i < 0) {
			this->i = static_cast<int>(this->vec.size()) - 1;
		}

		return this->vec.at(i);
	}

	/**
	 * Gets the current element.
	 */
	T& current() {
		return this->vec[this->i];
	}

	/**
	 * Advance to the next element in the backing vector, and return it. When
	 * the end is reached, it will return the first element.
	 */
	T& next() {
		this->i++;
		if (this->i > static_cast<int>(this->vec.size()) - 1) {
			this->i = 0;
		}

		return this->vec.at(this->i);
	}
private:
	std::vector<T> vec;

	// Note: we use a signed integer to reference the index because we need to
	// go negative in order to rotate to the end.
	int i = 0;
};


#endif // CIRCULAR_LIST_HPP
