#include <iostream>

#include "car.hpp"

namespace car {

Car::Car(const std::uint32_t& car_num_, const double& speed_,
         const std::string& str_tire)
    : car_num(car_num_), speed(speed_), tire(str_tire) {}

void Car::step() {
    tire.step();
}

}  // namespace car
