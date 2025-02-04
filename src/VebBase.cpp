#include "veb.h"
#include <bitset>
#include <cstdint>

class VebBase : public VEB {
    private:
    uint32_t min;
    uint32_t max;
    std::bitset<16> base_bits;

    VebBase() {
        //some value outside base case handling
        min = 101;
        max = 101;
    };

    //potential issues if min and max get called
    //when there has been no inserts, but a base
    //VEB should never be empty in real use

    uint32_t max() override {
        return max;
    }

    uint32_t min() override {
        return min;
    }

    bool insert(uint32_t) override {
        if (min == 101) {
            min = x;
            max = x;
            has_min = true;
            has_max = true;
        }
        if (x < min) {
            min = x;
        }
        if (x > max) {
            max = x;
        }
        base_bits[x] = 1;
        return true;
    }

    bool query(uint32_t x) override {
        return base_bits[x];
    }
    
    long successor(uint32_t x) override {
        for (int i = x + 1; i < 16; i++) {
            if (base_bits[i]) {
                return i;
            }
        }
        return -1;
    }
};