#include <algorithm>
#include <iostream>

#include "race.hpp"

namespace race {

Race::Race(circuit::Circuit&& circuit_, const std::uint32_t& num_of_laps_)
    : circuit(circuit_), num_of_laps(num_of_laps_) {}

void Race::add_car(car::Car&& arg_car) {
    if (!grid_fixed) {
        car_state_list.push_back({1, 0, arg_car});
    } else {
        std::cerr << "Grid fixed, no more cars" << std::endl;
        std::exit(1);
    }
}

void Race::show_standings() const {
    std::uint32_t idx{};

    std::cout << "Pos.\tNo.\t" << std::endl;

    // sort by Car::lap + CarState::distance
    // std::sort(std::begin(car_state_list), std::end(car_state_list),
    //           [&](CarState car_state1, CarState car_state2) {
    //               return car_state1 > car_state2;
    //           });

    for (const auto& car_state : car_state_list) {
        std::cout << ++idx << "\t" << car_state.car.car_num << std::endl;
    }
}

void Race::formation_lap() { grid_fixed = true; }

void Race::start() {
    for (double timer{}; timer < 0.1; timer += config::tick) {
        step();
    }
}

void Race::step() {
    for (auto& car_state : car_state_list) {
        double&& delta{car_state.car.step()};
        car_state.distance += delta;
        if (car_state.distance >= circuit.course_length) {
            car_state.distance = car_state.distance - circuit.course_length;
            car_state.car.lap++;
        }

        std::cout << car_state.car.car_num << "\t" << car_state.distance
                  << std::endl;
    }
}

}  // namespace race