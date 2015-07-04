#include <limits>


class Nano {
public:
    Nano& operator+=(Nano& rhs) {
        dealt += rhs.dealt;
        received += rhs.received;
        return *this;
    }

    void add() {

    }

    int getTotalDealt() {return dealt.total;}
    int getCountDealt() {return dealt.count;}
    int getMaxDealt() {return dealt.max;}
    int getMinDealt() {return dealt.min;}

    int getTotalReceived() {return received.total;}
    int getCountReceived() {return received.count;}
    int getMaxReceived() {return received.max;}
    int getMinReceived() {return received.min;}

private:
    struct nanoInfo {
        nanoInfo& operator+=(nanoInfo& rhs) {
            total += rhs.total;
            count += rhs.count;
            if (rhs.max > max) {
                max = rhs.max;
            }
            if (rhs.min < min) {
                min = rhs.min;
            }
            return *this;
        }

        int total = 0;
        int count = 0;
        int max = -1;
        int min = std::numeric_limits<int>::max();
    };

    nanoInfo dealt;
    nanoInfo received;
};
