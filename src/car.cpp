#include <iostream>

#include "car.hpp"
#include "config.hpp"

namespace car {

Car::Car(const double& speed_, const std::uint32_t& car_num_,
         const std::vector<TireCompound>& tire_strategy)
    : speed(speed_), car_num(car_num_), tire(tire_strategy[0]) {}

void Car::change_tire(const std::string& str) {}

double Car::step(const double& distance_gap, const car::Car& forerunner) {
    tire.step();

    double gain = speed * tire.performance;  // distance per tick

    if (lap == forerunner.lap && distance_gap <= config::BLOCKABLE_RANGE) {
        gain *= config::BLOCKED_PERFORMANCE_RATIO;
    }

    return gain;
}

void Car::next_lap() {
    lap++;
    tire.next_lap();
}

}  // namespace car