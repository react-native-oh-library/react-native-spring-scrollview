//
// Created by MatzZze on 05.11.2022.
//

#ifndef EVENTBUS_EVENTBUS_H
#define EVENTBUS_EVENTBUS_H

#include <memory>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>
#include "EventHandler.h"
#include "Event.h"

namespace EventBus {

typedef size_t EBHandlerID;

class EventBus {
public:
    static EventBus *getInstance();
    static void cleanUp();

    virtual ~EventBus();

    EBHandlerID registerHandler(std::shared_ptr<EventHandlerBase> &handler);
    void unregisterHandler(EBHandlerID id);
    void fire(std::shared_ptr<Event> &event);
    void fireAndForget(const std::shared_ptr<Event> &event);
    void setEvent(std::shared_ptr<Event> &event);
    std::shared_ptr<Event> getEvent();

protected:
    void handlingThread(bool &initialized);
    inline void dispatchEvent(std::shared_ptr<Event> &event) const;
    inline void handleInsertionsAndRemovals();

    EBHandlerID idCounter;
    std::unordered_map<EBHandlerID, std::shared_ptr<EventHandlerBase>> registrations;
    std::vector<std::pair<EBHandlerID, std::shared_ptr<EventHandlerBase>>> temporaryRegistrations;
    std::vector<EBHandlerID> temporaryRegistrationsRemovals;
    std::mutex registrationLock;
    std::mutex condMtx;
    std::mutex queueMtx;
    std::mutex threadMtx;
    std::mutex tmpRegistrationsMtx;
    std::mutex tmpRegistrationsRemovalsMtx;
    std::condition_variable eventCond;
    std::condition_variable threadCond;
    std::unique_ptr<std::thread> threadPtr;
    std::vector<std::shared_ptr<Event>> eventQueue;
    bool stop;
    std::shared_ptr<Event> event;

private:
    EventBus();
    static std::unique_ptr<EventBus> instance;
};

} // namespace EventBus

#endif // EVENTBUS_EVENTBUS_H