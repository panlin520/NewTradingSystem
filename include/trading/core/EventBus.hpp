#pragma once

#include "trading/core/Event.hpp"

#include <functional>
#include <vector>

namespace CMETradingSystem::Core {

class EventBus
{
public:
    using Handler = std::function<void(const Event&)>;

    void subscribe(Handler handler);

    void publish(const Event& event) const;

private:
    std::vector<Handler> handlers_;
};

}
