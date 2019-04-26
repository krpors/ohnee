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

	T& next() {
		T& ret = *this->it;

		this->it++;
		if (this->it == this->vec.end()) {
			this->it = this->vec.begin();
		}

		return ret;
	}
private:
	std::vector<T> vec;
	typename std::vector<T>::iterator it;
};


#endif // CIRCULAR_LIST_HPP
