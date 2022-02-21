#ifndef CAR_HPP
#define CAR_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "tire.hpp"

namespace car {

class Car {
   private:
    const double speed;  // e.g. 1, 0.75

    const TireStrategy tire_strategy;

    std::uint32_t tire_cnt{1};

   public:
    const std::uint32_t car_num;

    std::uint32_t lap{1};

    Tire tire;

    Car(const double&, const std::uint32_t&, const TireStrategy&);

    Car(const Car&);

    Car& operator=(const Car&);

    void change_tire(const std::string&);

    double step(const double&, const car::Car&);

    void next_lap();
};

}  // namespace car

#endif