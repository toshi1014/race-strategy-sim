#include <iostream>

#include "race.hpp"

namespace race {

Race::Race(circuit::Circuit&& circuit_, const std::uint32_t& num_of_laps_)
    : circuit(circuit_), num_of_laps(num_of_laps_) {}

void Race::add_car(car::Car&& arg_car) {
    if (!grid_fixed) {
        car_list.push_back(arg_car);
    } else {
        std::cerr << "Grid fixed, no more cars" << std::endl;
        std::exit(1);
    }
}

void Race::show_standings() const {
    std::uint32_t idx{};

    std::cout << "Pos.\tNo.\t" << std::endl;

    for (const auto& car : car_list) {
        std::cout << ++idx << "\t" << car.car_num << std::endl;
    }
}

void Race::formation_lap() { grid_fixed = true; }

void Race::start() {
    for (double timer{}; timer < 0.1; timer += config::tick) {
        for (auto& car : car_list) {
            car.step();
        }
    }
}

}  // namespace race