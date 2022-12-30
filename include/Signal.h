//
// Created by ASUS on 31.12.2022.
//

#ifndef DISCRETEMATHLAB3_SIGNAL_H
#define DISCRETEMATHLAB3_SIGNAL_H

#include <map>
#include <set>
#include <functional>

class Signal {
public:
    Signal() = default;

    std::size_t addSlot(const std::function<void(void)> &);

    bool deleteSlot(std::size_t id);

    [[nodiscard]] std::function<void(void)> getSlot(std::size_t id) const;

    std::size_t getSlotsCount();

    void operator()() const;

private:
    std::size_t id;
    std::map<std::size_t, std::function<void(void)>> delegates;
};

class SignalContainer {
public:
    void emit(std::size_t id) const;

    bool add(std::size_t id, const Signal &signal);

    [[nodiscard]] bool contains(std::size_t id) const;

    Signal &get(std::size_t id);

    [[nodiscard]] std::size_t getSignalsCount() const;

    [[nodiscard]] std::set<std::size_t> getKeySet() const;

private:
    std::map<std::size_t, Signal> signals;
};


#endif //DISCRETEMATHLAB3_SIGNAL_H
