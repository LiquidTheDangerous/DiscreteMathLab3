//
// Created by ASUS on 31.12.2022.
//

#include <Signal.h>
#include <stdexcept>

std::size_t Signal::addSlot(const std::function<void(void*)>& func) {
    this->delegates.emplace(this->id,func);
    return this->id++;
}

std::function<void(void*)> Signal::getSlot(std::size_t _id) const {
    auto iter = this->delegates.find(id);
    if (iter == this->delegates.end()){
        throw std::runtime_error("Index out of range");
    }
    return iter->second;
}

std::size_t Signal::getSlotsCount() {
    return this->delegates.size();
}

void Signal::operator()(void* param) const {
    for (auto& d : this->delegates){
        d.second(param);
    }
}

bool Signal::deleteSlot(std::size_t _id) {
    auto iter = this->delegates.find(_id);
    if (iter == this->delegates.end()){
        return false;
    }
    this->delegates.erase(iter);
    return true;
}

void SignalContainer::emit(std::size_t id, void *param) const {
    auto iter = this->signals.find(id);
    if (iter != this->signals.end()){
        iter->second.operator()(param);
    }
}

bool SignalContainer::add(std::size_t id, const Signal &signal) {
    auto pair = this->signals.emplace(id, signal);
    return pair.second;
}

Signal &SignalContainer::get(std::size_t id) {
    auto iter = this->signals.find(id);
    if (iter == this->signals.end()){
        auto pair = this->signals.emplace(id,Signal());
        iter = pair.first;
    }
    return iter->second;
}

std::size_t SignalContainer::getSignalsCount() const {
    return this->signals.size();
}

std::set<std::size_t> SignalContainer::getKeySet() const {
    std::set<std::size_t> result;
    for (auto& pair : this->signals){
        result.emplace(pair.first);
    }
    return result;
}

bool SignalContainer::contains(std::size_t id) const {
    return this->signals.contains(id);
}
