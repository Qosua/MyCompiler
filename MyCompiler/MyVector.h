#pragma once

#include <iostream>

class MyVector {

public:
	MyVector();
	MyVector(const size_t& elemCount, const int& elem);
	~MyVector();

	void reserve(const size_t& elemCount);

	void pushBack(const int& elem);
	void insert(const size_t& elemPosBefore, const int& elem);

	void popBack();
	void erase(const size_t& elemPos);

	int& operator[](const size_t& elemPos) const;
	int& operator[](const size_t& elemPos);

	size_t size() const;
	size_t resevedSize() const;

private:
	int* m_arr;
	size_t m_visibleSize;
	size_t m_actualSize;


};

