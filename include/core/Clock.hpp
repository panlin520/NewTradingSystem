#pragma once

#include <cstdint>

namespace CMETradingSystem::Core {

class Clock
{
public:
    uint64_t now() const;
};

}
