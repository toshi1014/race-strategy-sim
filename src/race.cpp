#include <algorithm>
#include <iostream>
#include <utility>

#include "config.hpp"
#include "race.hpp"

namespace race {

Race::Race(circuit::Circuit&& circuit_, const std::uint32_t& num_of_laps_)
    : circuit(circuit_), num_of_laps(num_of_laps_) {}

void Race::add_car(car::Car&& arg_car) {
    if (!grid_fixed) {
        std::uint32_t&& position_now = car_state_list.size() + 1;

        car_state_list.push_back({
            position_now,
            -static_cast<double>(position_now * config::GRID_GAP),
            arg_car,
        });

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
    for (double timer{}; timer < 0.9 && !bool_checkered;
         timer += config::TICK) {
        std::cout << "\n\ntime:\t" << timer << "\n" << std::endl;
        step();
    }
}

void Race::next_lap(CarState& car_state) {
    car_state.distance = car_state.distance - circuit.course_length;
    car_state.car.next_lap();
    if (is_checkered(car_state)) bool_checkered = true;

    std::cout << "next lap: " << car_state.car.car_num << std::endl;
}

void Race::step() {
    // for (auto& car_state : car_state_list) {
    for (std::uint32_t idx{}; idx < car_state_list.size(); idx++) {
        auto& car_state = car_state_list[idx];

        const auto& forerunner = Race::get_forerunner(idx);

        const double&& distance_gap =
            Race::get_distance_gap(forerunner, car_state);

        // double&& delta{car_state.car.step(forerunner)};
        double&& delta{car_state.car.step(distance_gap, forerunner.car)};

        car_state.distance += delta;

        if (car_state.distance >= circuit.course_length)
            Race::next_lap(car_state);

        std::cout << car_state.car.car_num << "\t" << car_state.distance
                  << std::endl;
    }
}

bool Race::is_checkered(const CarState& car_state) const {
    return car_state.car.lap > num_of_laps;
}

CarState Race::get_forerunner(std::uint32_t& idx) const {
    if (idx == 0) {
        return car_state_list[car_state_list.size() - 1];  // -1: length to idx
    } else {
        return car_state_list[idx - 1];
    }
}

double Race::get_distance_gap(const CarState& forerunner,
                              const CarState& self) const {
    double gap = forerunner.distance - self.distance;
    if (gap < 0) {
        return circuit.course_length + gap;
    } else {
        return gap;
    }
}

}  // namespace race