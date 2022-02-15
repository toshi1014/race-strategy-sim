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

constexpr double tick{0.1};

std::map<car::TireCompound, TireSpec> tire_spec_dict = {
    {car::TireCompound::hard, {30, 0.8}},
    {car::TireCompound::medium, {20, 0.9}},
    {car::TireCompound::soft, {10, 1.}},
};

}  // namespace config

#endif