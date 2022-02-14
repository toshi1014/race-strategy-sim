#ifndef TIRE
#define TIRE

#include <string>

namespace car {

class Tire {
   private:
    double performance{1.};
    double base_performance;

    std::uint32_t age{};
    std::uint32_t lifespan;

    enum struct Compound {
        hard,
        medium,
        soft,
    } compound;

   public:
    explicit Tire(const std::string&);

    void step();

    friend class Car;
};

}  // namespace car

#endif