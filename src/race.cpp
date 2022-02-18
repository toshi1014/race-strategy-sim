#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <utility>

#include "config.hpp"
#include "race.hpp"

namespace race {

CarState& CarState::operator=(const CarState& other) {
    position = other.position;
    distance = other.distance;
    car = other.car;
    return *this;
}

Race::Race(circuit::Circuit&& circuit_, const std::uint32_t& num_of_laps_)
    : circuit(circuit_), num_of_laps(num_of_laps_) {}

void Race::add_car(car::Car&& arg_car) {
    if (!grid_fixed) {
        std::uint32_t position_now = car_state_list.size() + 1;

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

const CarState& Race::get_car_state_by_position(
    const std::uint32_t& position) const {
    const auto car_state_it =
        std::find_if(std::begin(car_state_list), std::end(car_state_list),
                     [&](const CarState& car_state) {
                         return car_state.position == position;
                     });

    return *car_state_it;
}

void Race::show_standings() const {
    std::cout << "Pos.\tNo.\t" << std::endl;

    for (std::uint32_t pos_i{1}; pos_i <= car_state_list.size(); pos_i++) {
        const CarState& car_state = Race::get_car_state_by_position(pos_i);
        std::cout << pos_i << "\t" << car_state.car.car_num << std::endl;
    }
}

void Race::formation_lap() { grid_fixed = true; }

void Race::start() {
    for (double timer{}; timer < 1. && !bool_checkered; timer += config::TICK) {
        std::cout << "\n\ntime:\t" << timer << "\n" << std::endl;

        step();

        for (const auto& car_state : car_state_list) {
            std::cout << "Num. " << car_state.car.car_num
                      << "\tdistance: " << car_state.distance << "\tPos. "
                      << car_state.position << std::endl;
        }
    }
}

void Race::next_lap(CarState& car_state) {
    car_state.distance = car_state.distance - circuit.course_length;
    car_state.car.next_lap();

    if (is_checkered(car_state)) {
        bool_checkered = true;
        std::cout << "\n\n========= Checkered =========\n" << std::endl;
    } else {
        std::cout << "No. " << car_state.car.car_num
                  << "\tnext lap: " << car_state.car.lap << std::endl;
    }
}

void Race::step() {
    for (std::uint32_t pos_i{1}; pos_i <= car_state_list.size(); pos_i++) {
        CarState& car_state =
            const_cast<CarState&>(Race::get_car_state_by_position(pos_i));

        double delta{};

        if (car_state.position == 1) {
            delta =
                car_state.car.step(config::BLOCKABLE_RANGE + 1., car_state.car);

        } else {
            CarState& forerunner = Race::get_forerunner(car_state);
            const double distance_gap =
                Race::get_distance_gap(forerunner, car_state);

            delta = car_state.car.step(distance_gap, forerunner.car);

            if (delta >= distance_gap) Race::overtake(forerunner, car_state);
        }

        car_state.distance += delta;

        if (car_state.distance >= circuit.course_length)
            Race::next_lap(car_state);
    }
}

bool Race::is_checkered(const CarState& car_state) const {
    return car_state.car.lap > num_of_laps;
}

CarState& Race::get_forerunner(const CarState& self) {
    const auto it =
        std::find_if(std::begin(car_state_list), std::end(car_state_list),
                     [&](const CarState& car_state) {
                         return car_state.position == self.position - 1;
                     });
    return *it;
}

double Race::get_distance_gap(const CarState& forerunner,
                              const CarState& self) const {
    const double gap{std::abs(forerunner.distance - self.distance)};
    return std::min(gap, circuit.course_length - gap);
}

void Race::overtake(CarState& forerunner, CarState& overtaking_car) {
    if (forerunner.car.lap == overtaking_car.car.lap) {
        std::cout << "Num. " << overtaking_car.car.car_num << " passed Num."
                  << forerunner.car.car_num << "\n"
                  << std::endl;

        forerunner.position++;
        overtaking_car.position--;
    }
}

}  // namespace race