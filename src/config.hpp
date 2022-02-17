#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdint>
#include <map>

#include "tire.hpp"

namespace {

struct TireSpec {
    std::uint32_t lifespan;
    double base_performance;
};

}  // namespace


namespace config {

constexpr double TICK{0.1};

constexpr std::uint32_t GRID_GAP = 1;

constexpr double BLOCKABLE_RANGE = 3.;

constexpr double BLOCKED_PERFORMANCE_RATIO = 0.8;

const std::map<car::TireCompound, TireSpec> TIRE_SPEC_DICT = {
    {car::TireCompound::hard, {30, 0.8}},
    {car::TireCompound::medium, {20, 0.9}},
    {car::TireCompound::soft, {10, 1.}},
};

}  // namespace config

#endif