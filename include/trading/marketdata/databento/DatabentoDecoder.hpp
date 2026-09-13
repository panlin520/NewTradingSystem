#pragma once

namespace CMETradingSystem::MarketData::Databento
{

// Databento decoder abstraction.
//
// Responsibility:
// - Wrap Databento SDK or custom DBN parser in the future.
// - Convert raw DBN records into an internal readable format.
//
// Not responsible for:
// - EventQueue
// - Engine
// - OrderBook
// - Strategy

class DatabentoDecoder
{
public:

    DatabentoDecoder() = default;
    ~DatabentoDecoder() = default;

    // Decoder initialization will be implemented
    // after Databento SDK dependency is added.
    bool open();

};

}
