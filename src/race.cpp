#include <algorithm>
#include <iostream>
#include <utility>

#include "race.hpp"

namespace race {

Race::Race(circuit::Circuit&& circuit_, const std::uint32_t& num_of_laps_)
    : circuit(circuit_), num_of_laps(num_of_laps_) {}

void Race::add_car(car::Car&& arg_car) {
    if (!grid_fixed) {
        std::uint32_t&& position_now = car_state_list.size() + 1;
        car_state_list.push_back({position_now, 0, arg_car});
    } else {
        std::cerr << "Grid fixed, no more cars" << std::endl;
        std::exit(1);
    }
}

void Race::show_standings() const {
    std::cout << "Pos.\tNo.\t" << std::endl;

    // if car_state == position_idx, print car number
    for (std::uint32_t pos_i{1}; pos_i <= car_state_list.size(); pos_i++) {
        auto car_state_it =
            std::find_if(std::begin(car_state_list), std::end(car_state_list),
                         [&](const CarState& car_state) {
                             return car_state.position == pos_i;
                         });

        std::cout << pos_i << "\t" << (*car_state_it).position << std::endl;
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
            car_state.car.next_lap();
        }

        std::cout << car_state.car.car_num << "\t" << car_state.distance
                  << std::endl;
    }
}

}  // namespace race