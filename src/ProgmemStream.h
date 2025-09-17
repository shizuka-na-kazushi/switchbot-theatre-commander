#ifndef __PROGMEM_STREAM_H__
#define __PROGMEM_STREAM_H__

#include <Arduino.h>
#include <pgmspace.h>

class ProgmemStream : public Stream {
public:
    ProgmemStream(const uint8_t* data, size_t length) : data(data), length(length), position(0) {}

    int available() override {
        return length - position;
    }

    int read() override {
        if (position < length) {
            return pgm_read_byte_near(data + position++);
        } else {
            return -1; // No more data
        }
    }

    int peek() override {
        if (position < length) {
            return pgm_read_byte_near(data + position);
        } else {
            return -1; // No more data
        }
    }

    void flush() override {
        // No-op for this implementation
    }

    size_t write(uint8_t) override {
        // Writing is not supported in this implementation
        return 0;
    }

private:
    const uint8_t* data;
    size_t length;
    size_t position;
};

#endif // !__PROGMEM_STREAM_H__