#pragma once

#include <cstdint>

namespace CMETradingSystem::Core {

struct Event
{
    uint64_t timestamp{0};
    virtual ~Event() = default;
};

}
