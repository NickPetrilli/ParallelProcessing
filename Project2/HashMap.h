#pragma once
#include "KeyValue.h"

class HashMap
{
private:
	const int INITIAL_CAPACITY = 8;
	const float LOAD_FACTOR = 0.75;

	int size, capacity;
	KeyValue* array;

	void checkResize();
	int hash(std::string);
	int find(std::string);

public:
	HashMap();
	~HashMap();

	int getSize();

	void put(std::string, int);
	int get(std::string);
	void increment(std::string);
	bool contains(std::string);
	void addTo(std::string, int);

	KeyValue* getAll();
};