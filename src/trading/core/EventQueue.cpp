#include "trading/core/EventQueue.hpp"

namespace CMETradingSystem::Core {

void EventQueue::push(std::unique_ptr<Event> event)
{
    if (event)
    {
        events_.push(std::move(event));
    }
}

std::unique_ptr<Event> EventQueue::pop()
{
    if (events_.empty())
    {
        return nullptr;
    }

    auto event = std::move(events_.front());
    events_.pop();

    return event;
}

bool EventQueue::empty() const
{
    return events_.empty();
}

std::size_t EventQueue::size() const
{
    return events_.size();
}

}
