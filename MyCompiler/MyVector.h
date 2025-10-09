#pragma once

#include <iostream>

template<typename T>
class MyVector {

public:
	MyVector() {
		m_visibleSize = 0;
		m_actualSize = 2;

		m_arr = new T[m_actualSize]();
	}
	MyVector(const size_t& elemCount, const T& elem) {
		m_visibleSize = elemCount;
		m_actualSize = elemCount;

		m_arr = new T[m_actualSize];

		for (int i = 0; i < elemCount; ++i)
			m_arr[i] = elem;
	}
	~MyVector() {
		delete[] m_arr;
		m_arr = nullptr;
	}

	void reserve(const size_t& elemCount) {

		if (elemCount <= m_actualSize)
			return;

		m_actualSize = elemCount;


		if (!m_arr) {
		T* copyArr = new T[m_actualSize]();

			for (int i = 0; i < m_visibleSize; ++i)
				copyArr[i] = m_arr[i];

			if (m_arr)
				delete[] m_arr;

			m_arr = copyArr;
			copyArr = nullptr;
		}
		else {
			m_arr = new T[m_actualSize]();
		}

	}

	void pushBack(const T& elem) {

		if (m_visibleSize + 1 > m_actualSize)
			reserve(m_actualSize * 2);

		m_arr[m_visibleSize] = elem;
		m_visibleSize += 1;
	}
	void insert(const size_t& elemPosBefore, const T& elem) {

		if (elemPosBefore > m_visibleSize)
			return;

		if (elemPosBefore == m_visibleSize) {
			pushBack(elem);
			return;
		}

		T* copyArr = new T[m_actualSize + 1];

		for (int i = 0, j = 0; i < m_visibleSize + 1; ++i) {

			if (i == elemPosBefore) {
				copyArr[i] = elem;
				continue;
			}

			copyArr[i] = m_arr[j];
			j += 1;

		}

		delete[] m_arr;
		m_arr = copyArr;
		copyArr = nullptr;

		m_visibleSize += 1;
		m_actualSize += 1;

	}

	void popBack() {
		m_arr[m_visibleSize - 1] = T();
		m_visibleSize -= 1;
	}
	void erase(const size_t& elemPos) {

		if (elemPos >= m_visibleSize)
			return;

		T* copyArr = new T[m_actualSize - 1];

		for (int i = 0, j = 0; i < m_visibleSize; ++i) {

			if (i == elemPos)
				continue;

			copyArr[j] = m_arr[i];
			j += 1;

		}

		delete[] m_arr;
		m_arr = copyArr;
		copyArr = nullptr;

		m_visibleSize -= 1;
		m_actualSize -= 1;

	}

	T& operator[](const size_t& elemPos) const {
		return m_arr[elemPos];
	}
	T& operator[](const size_t& elemPos) {
		return m_arr[elemPos];
	}
	MyVector& operator=(const MyVector& copy) {
		m_visibleSize = copy.m_visibleSize;
		m_actualSize = copy.m_actualSize;

		if (m_arr)
			delete[] m_arr;

		m_arr = new T[m_actualSize];

		for (int i = 0; i < copy.m_visibleSize ; ++i) {
			m_arr[i] = copy.m_arr[i];
		}

		return *this;
	}

	size_t size() const {
		return m_visibleSize;
	}
	size_t resevedSize() const {
		return m_actualSize;
	}

private:
	T* m_arr;
	size_t m_visibleSize = 0;
	size_t m_actualSize = 0;

};
