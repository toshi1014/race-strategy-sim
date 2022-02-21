#ifndef TIRE
#define TIRE

#include <string>
#include <tuple>
#include <vector>

namespace car {

enum struct TireCompound {
    hard,
    medium,
    soft,
};

struct CompoundLap {
    const TireCompound compound;
    const std::uint32_t lap;
};

struct TireStrategy {
    const std::vector<CompoundLap> tire_strategy;

    const CompoundLap& at(const std::uint32_t&) const;

    std::uint32_t size() const;
};

class Tire {
   private:
    double performance{1.};

    const double base_performance;

    const std::uint32_t lifespan;

   public:
    const TireCompound compound;

    std::uint32_t age{1};

    explicit Tire(const TireCompound&);

    void step();

    void next_lap();

    std::string get_compound_str() const;

    friend class Car;
};

}  // namespace car

#endif