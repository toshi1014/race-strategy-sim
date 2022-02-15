#ifndef TIRE
#define TIRE

#include <string>

namespace car {

enum struct TireCompound {
    hard,
    medium,
    soft,
};

class Tire {
   private:
    double performance{1.};
    const double base_performance;

    std::uint32_t age{};
    const std::uint32_t lifespan;

   public:
    const TireCompound compound;

    explicit Tire(const TireCompound&);

    void step();

    void next_lap();

    friend class Car;
};

}  // namespace car

#endif