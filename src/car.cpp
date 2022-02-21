#include <iostream>

#include "car.hpp"
#include "config.hpp"

namespace car {

Car::Car(const double& speed_, const std::uint32_t& car_num_,
         const TireStrategy& tire_strategy_)
    : speed(speed_),
      tire_strategy(tire_strategy_),
      car_num(car_num_),
      tire(tire_strategy_.at(0).compound)  // put start tire on
{
    std::cout << "constructed" << std::endl;
}

Car::Car(const Car& other)
    : speed(other.speed), car_num(other.car_num), tire(other.tire) {
    std::cout << "copied" << std::endl;
}

Car& Car::operator=(const Car& other) {
    std::cout << "operator=" << std::endl;
    return *this;
}

void Car::change_tire(const std::string& str) {}

double Car::step(const double& distance_gap, const car::Car& forerunner) {
    tire.step();

    double gain{speed * tire.performance};  // distance per tick

    if (distance_gap <= config::BLOCKABLE_RANGE && lap <= forerunner.lap) {
        gain *= config::BLOCKED_PERFORMANCE_RATIO;
    }

    std::cout << "tire size " << tire_strategy.size() << std::endl;
    return gain;
}

void Car::next_lap() {
    lap++;

    // if (lap == tire_strategy.at(tire_cnt).lap) {
    // } else {
    //     tire.next_lap();
    // }
}

}  // namespace car