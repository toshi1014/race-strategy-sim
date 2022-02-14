#include <cstdint>

namespace config {

constexpr double tick{0.1};

namespace car {

// tire durability
constexpr std::uint32_t hard_tire_lifespan{30};    // laps
constexpr std::uint32_t medium_tire_lifespan{20};  // laps
constexpr std::uint32_t soft_tire_lifespan{10};    // laps

}  // namespace car

}  // namespace config