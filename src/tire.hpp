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