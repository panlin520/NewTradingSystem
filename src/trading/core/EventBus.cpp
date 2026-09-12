#include "trading/core/EventBus.hpp"

namespace CMETradingSystem::Core {

void EventBus::subscribe(Handler handler)
{
    handlers_.push_back(std::move(handler));
}

void EventBus::publish(const Event& event) const
{
    for (const auto& handler : handlers_)
    {
        handler(event);
    }
}

}
