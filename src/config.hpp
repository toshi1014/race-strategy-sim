#include <cstdint>
#include <map>

namespace {

struct TireSpec {
    std::uint32_t lifespan;
    double base_performance;
};

}  // namespace

namespace config {

constexpr double tick{0.1};

namespace car {

std::map<std::string, TireSpec> tire_spec_dict = {
    {"hard", {30, 0.8}},
    {"medium", {20, 0.9}},
    {"soft", {10, 1.}},
};

}  // namespace car

}  // namespace config