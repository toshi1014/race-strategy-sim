#ifndef CAR_HPP
#define CAR_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "tire.hpp"

namespace car {

class Car {
   private:
    const double speed;  // e.g. 1, 0.75

    std::uint32_t tire_cnt{1};

   public:
    const TireStrategy tire_strategy;

    const std::uint32_t car_num;

    std::uint32_t lap{1};

    std::shared_ptr<Tire> tire_ptr;

    Car(const double&, const std::uint32_t&, const TireStrategy&);

    Car(const Car&);

    Car& operator=(const Car&);

    void change_tire(const std::uint32_t& tire_cnt);

    double step(const double&, const car::Car&);

    void next_lap();
};

}  // namespace car

#endif