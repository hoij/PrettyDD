#ifndef NANO_H
#define NANO_H


#include <limits>
#include <string>


class Nano {
public:
    Nano& operator+=(const Nano& rhs) {
        dealt += rhs.dealt;
        received += rhs.received;
        return *this;
    }

    void add(LineInfo& li, std::string playerType) {
        if (playerType == "dealer") {
            dealt.count++;
            dealt.total += li.amount;
            if (li.amount < dealt.min) {
                dealt.min = li.amount;
            }
            if (li.amount > dealt.max) {
                dealt.max = li.amount;
            }
        }
        else if (playerType == "receiver") {
            received.count++;
            received.total += li.amount;
            if (li.amount < received.min) {
                received.min = li.amount;
            }
            if (li.amount > received.max) {
                received.max = li.amount;
            }
        }
    }

    int getTotalDealt() const {return dealt.total;}
    int getCountDealt() const {return dealt.count;}
    int getMaxDealt() const {return dealt.max;}
    int getMinDealt() const {return dealt.min;}

    int getTotalReceived() const {return received.total;}
    int getCountReceived() const {return received.count;}
    int getMaxReceived() const {return received.max;}
    int getMinReceived() const {return received.min;}

private:
    struct nanoInfo {
        nanoInfo& operator+=(const nanoInfo& rhs) {
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

inline Nano operator+(Nano lhs, const Nano& rhs) {
    return lhs += rhs;
}

#endif  // NANO_H
