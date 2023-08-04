
#ifndef SCROLLING_BUFFER_H
#define SCROLLING_BUFFER_H

#include <list>
#include <Arduino.h>

template <typename T> class ScrollingBuffer {

typedef bool (*comp_func)(T one, T other);

public:

    std::list<T> buffer;
    
    ScrollingBuffer() {
    }

    void addToBuffer(T s) {
        buffer.push_back(s);
    }

    void shiftForward() {
        if(!buffer.empty()) {
            T s = buffer.front();
            buffer.pop_front();
            buffer.push_back(s);
        }
    }

    T at(int index) {
        int i = 0;

        if(SERIAL_DEBUG) {
            Serial.println("Retrieving object at index: " + String(index));
        }

        if(!buffer.empty()) {
            for(const auto v : buffer){
                if(i == index) {
                    return v;
                }
                i += 1;
            }
            return buffer.back();
        }
        return NULL;
    }

    int size() {
        return buffer.size();
    }

    std::list<T> getBuffer() {
        return buffer;
    }

    void sort(comp_func func) {
        buffer.sort(func);
    }

    bool empty() {
        return buffer.empty();
    }

    void clear() {
        buffer.clear();
    }
    
};

#endif