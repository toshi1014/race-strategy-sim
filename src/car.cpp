#include <iostream>

#include "car.hpp"

namespace car {

Car::Car(const double& speed_, const std::uint32_t& car_num_,
         const std::vector<TireCompound>& tire_strategy)
    : speed(speed_), car_num(car_num_), tire(tire_strategy[0]) {}

void Car::change_tire(const std::string& str) {}

double Car::step() {
    tire.step();

    // distance per tick
    return speed * tire.performance;
}

void Car::next_lap() {
    lap++;
    tire.next_lap();
}

}  // namespace car
