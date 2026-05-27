#pragma once
#define ARDUINO
#include <linkedlist.h>
#include <Arduino.h>
#include <ATcommands.h>
#include <client.h>

class dataPackage{
    byte* data;
    size_t size;
    public:
    dataPackage(byte* _data,size_t _size) : data(_data),size(_size) {}
    byte* get(const String str,size_t &outputSize);
    void printPackage();
};