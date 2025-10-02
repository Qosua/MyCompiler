#include "MyVector.h"

MyVector::MyVector() {
	m_visibleSize = 0;
	m_actualSize = 2;

	m_arr = new int[m_actualSize]();
}

MyVector::MyVector(const size_t& elemCount, const int& elem) {
	m_visibleSize = elemCount;
	m_actualSize = elemCount;

	m_arr = new int[m_actualSize];
}

MyVector::~MyVector() {
	delete m_arr;
	m_arr = nullptr;
}

void MyVector::reserve(const size_t& elemCount) {
	
	if (elemCount <= m_actualSize)
		return;

	m_actualSize = elemCount;

	int* copyArr = new int[m_actualSize]();

	for (int i = 0; i < m_visibleSize; ++i)
		copyArr[i] = m_arr[i];

	if(m_arr)
		delete m_arr;

	m_arr = copyArr;
	copyArr = nullptr;

}

void MyVector::pushBack(const int& elem) {

	if (m_visibleSize + 1 >= m_actualSize)
		reserve(m_actualSize * 2);

	m_arr[m_visibleSize] = elem;
	m_visibleSize += 1;
}

void MyVector::insert(const size_t& elemPosBefore, const int& elem) {

	if (elemPosBefore > m_visibleSize)
		return;

	if (elemPosBefore == m_visibleSize) {
		pushBack(elem);
		return;
	}

	int* copyArr = new int[m_actualSize + 1];

	for (int i = 0, j = 0; i < m_visibleSize + 1; ++i) {

		if (i == elemPosBefore) {
			copyArr[i] = elem;
			continue;
		}

		copyArr[i] = m_arr[j];
		j += 1;

	}

	delete m_arr;
	m_arr = copyArr;
	copyArr = nullptr;

	m_visibleSize += 1;
	m_actualSize += 1;

}

void MyVector::popBack() {
	m_arr[m_visibleSize - 1] = int();
	m_visibleSize -= 1;
}

void MyVector::erase(const size_t& elemPos) {

	if (elemPos >= m_visibleSize)
		return;

	int* copyArr = new int[m_actualSize - 1];

	for (int i = 0, j = 0; i < m_visibleSize; ++i) {

		if (i == elemPos)
			continue;

		copyArr[j] = m_arr[i];
		j += 1;

	}

	delete m_arr;
	m_arr = copyArr;
	copyArr = nullptr;

	m_visibleSize -= 1;
	m_actualSize -= 1;

}

int& MyVector::operator[](const size_t& elemPos) const {
	return m_arr[elemPos];
}

int& MyVector::operator[](const size_t& elemPos) {
	return m_arr[elemPos];
}

size_t MyVector::size() const {
	return m_visibleSize;
}

size_t MyVector::resevedSize() const {
	return m_actualSize;
}


