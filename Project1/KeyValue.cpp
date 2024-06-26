#include "KeyValue.h"

KeyValue::KeyValue() : KeyValue("", 0)
{}

KeyValue::KeyValue(std::string key, int value)
{
	this->key = key;
	this->value = value;
}

std::string KeyValue::getKey()
{
	return this->key;
}

int KeyValue::getValue()
{
	return this->value;
}

void KeyValue::setKey(std::string newKey)
{
	this->key = newKey;
}

void KeyValue::setValue(int newValue)
{
	this->value = newValue;
}

void KeyValue::increment()
{
	this->value++;
}