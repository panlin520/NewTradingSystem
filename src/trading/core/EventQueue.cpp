#include "trading/core/EventQueue.hpp"

namespace CMETradingSystem::Core {

void EventQueue::push(const Event& event)
{
    events_.push(event);
}

bool EventQueue::pop(Event& event)
{
    if (events_.empty())
    {
        return false;
    }

    event = events_.front();
    events_.pop();

    return true;
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
