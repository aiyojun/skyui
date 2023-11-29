#ifndef BUS_H
#define BUS_H

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

namespace jlib {

//    template<typename T>
//    class Publisher {
//    public:
//
//    private:
//
//    };
//
//    template<typename T>
//    class Subscriber {
//    public:
//
//    private:
//
//    };
//
//    class Platform {
//    public:
//
//    private:
//        struct TopicGroup {
//            std::vector<>
//        };
//
//        std::map<std::string, > _;
//    };

//template<typename T>
//class IBusListener {
//public:
//    virtual void listen(const T& message) = 0;
//};
//
////using AutoBusListener = std::shared_ptr<IBusListener>;
//
//class Bus {
//public:
//
//    template<typename T>
//    void subscribe(const std::string& topic, IBusListener<T> *listener) {
//        if (listeners_.find(topic) == listeners_.end())
//            listeners_[topic] = {};
//        if (contains(topic, listener)) return;
//        listeners_[topic].emplace_back(listener);
//    }
//
//    void remove(const std::string& topic, IBusListener<T> *listener) {
//        if (listeners_.find(topic) == listeners_.end()) return;
//        auto group = listeners_[topic];
//        auto iter = std::find(group.begin(), group.end(), listener);
//        if (iter != group.end()) group.erase(iter);
//    }
//
//    template<typename T>
//    void publish(const std::string& topic, T message) {
//
//    }
//
//private:
//    void dispatch() {}
//
//    template<typename T>
//    bool contains(const std::string& topic, IBusListener<T> *listener) {
//        if (listeners_.find(topic) == listeners_.end()) return false;
//        auto group = listeners_[topic];
//        return std::find(group.begin(), group.end(), listener) != group.end();
//    }
//
//    std::map<std::string, std::vector<IBusListener<>*>> listeners_;
//};

} // jlib

#endif //BUS_H
