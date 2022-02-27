#include <iostream>

#include "car.hpp"
#include "config.hpp"

namespace car {

Car::Car(const double& speed_, const std::uint32_t& car_num_,
         const TireStrategy& tire_strategy_)
    : speed(speed_), tire_strategy(tire_strategy_), car_num(car_num_) {
    tire_ptr = tire_strategy.at(0)->tire_ptr;  // put start tire on
    std::cout << "Car constructed " << std::endl;
    tire_strategy.repr();
}

Car::Car(const Car& other)
    : speed(other.speed),
      tire_strategy(other.tire_strategy),
      car_num(other.car_num) {
    tire_ptr = other.tire_ptr;  // put start tire on
    std::cout << "Car copied" << std::endl;
}

Car& Car::operator=(const Car& other) {
    std::cout << "Car::operator=" << std::endl;
    return *this;
}

void Car::change_tire(const std::uint32_t& tire_cnt) {
    tire_ptr = tire_strategy.at(tire_cnt)->tire_ptr;
    std::cout << "No." << car_num << "\tchanged to "
              << tire_strategy.at(tire_cnt)->tire_ptr->get_compound_str()
              << " tire" << std::endl;
}

double Car::step(const double& distance_gap, const car::Car& forerunner) {
    tire_ptr->step();

    double gain{speed * tire_ptr->performance};  // distance per tick

    if (distance_gap <= config::BLOCKABLE_RANGE && lap <= forerunner.lap) {
        gain *= config::BLOCKED_PERFORMANCE_RATIO;
    }

    return gain;
}

void Car::next_lap() {
    if (lap == tire_strategy.at(tire_cnt - 1)->lap) {
        change_tire(tire_cnt);
        tire_cnt++;
    }

    tire_ptr->next_lap();

    lap++;
}

}  // namespace car