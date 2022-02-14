#include <iostream>

#include "car.hpp"

namespace car {

Car::Car(const double& speed_, const std::uint32_t& car_num_,
         const std::string& str_tire)
    : speed(speed_), car_num(car_num_), tire(str_tire) {}

double Car::step() {
    tire.step();

    // distance per tick
    return speed * tire.performance;
}

}  // namespace car
