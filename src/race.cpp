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
    checkered = other.checkered;
    std::cout << "CarState operator=" << std::endl;
    return *this;
}

Race::Race(circuit::Circuit&& circuit_, const std::uint32_t& num_of_laps_)
    : circuit(circuit_), num_of_laps(num_of_laps_) {}

void Race::add_car(car::Car&& arg_car) {
    if (!grid_fixed) {
        std::uint32_t position_now = car_state_list.size() + 1;

        std::cout << "car adding..." << std::endl;
        car_state_list.push_back({
            position_now, -static_cast<double>(position_now * config::GRID_GAP),
            arg_car,
            false,  // checkered
        });
        std::cout << "car added" << std::endl;
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
    std::cout << "Pos.\tNo.\tLap\tTire\tAge\n" << std::endl;

    for (std::uint32_t pos_i{1}; pos_i <= car_state_list.size(); pos_i++) {
        const CarState& car_state = Race::get_car_state_by_position(pos_i);
        std::cout << pos_i << "\t" << car_state.car.car_num << "\t"
                  << car_state.car.lap << "\t"
                  << car_state.car.tire_ptr->get_compound_str() << "\t"
                  << car_state.car.tire_ptr->age << "\t" << std::endl;
    }
}

void Race::formation_lap() { grid_fixed = true; }

void Race::start() {
    for (double timer{}; timer < 100. && !all_checkered;
         timer += config::TICK) {
        step();

        // {
        //     std::cout << "\n\ntime:\t" << timer << "\n" << std::endl;
        //     for (const auto& car_state : car_state_list) {
        //         std::cout << "No." << car_state.car.car_num
        //                   << "\tdistance: " << car_state.distance << "\tPos."
        //                   << car_state.position << std::endl;
        //     }
        // }
    }

    std::cout << "\n\n========= Checkered =========\n" << std::endl;
}

void Race::next_lap(CarState& car_state) {
    if (Race::is_checkered(car_state)) {
        std::cout << "No." << car_state.car.car_num << "\tcheckered"
                  << std::endl;
        checker_flg = true;
        car_state.checkered = true;
        all_checkered = Race::is_all_checkered();
    } else {
        car_state.distance = car_state.distance - circuit.course_length;
        car_state.car.next_lap();

        std::cout << "No." << car_state.car.car_num
                  << "\tnext lap: " << car_state.car.lap << std::endl;
    }
}

void Race::step() {
    for (std::uint32_t pos_i{1}; pos_i <= car_state_list.size(); pos_i++) {
        CarState& car_state =
            const_cast<CarState&>(Race::get_car_state_by_position(pos_i));

        if (car_state.checkered) continue;

        double delta{};

        if (pos_i == 1) {
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
    return car_state.car.lap >= num_of_laps || checker_flg;
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
    const double gap_base{std::abs(forerunner.distance - self.distance)};
    const double gap{std::min(gap_base, circuit.course_length - gap_base)};

    return gap + (forerunner.car.lap - self.car.lap) * circuit.course_length;
}

void Race::overtake(CarState& forerunner, CarState& overtaking_car) {
    if (!forerunner.checkered) {
        if (forerunner.car.lap == overtaking_car.car.lap) {
            std::cout << "No." << overtaking_car.car.car_num << "\tpassed No."
                      << forerunner.car.car_num << std::endl;

            forerunner.position++;
            overtaking_car.position--;
        }
    }
}

bool Race::is_all_checkered() const {
    for (const auto& car_state : car_state_list) {
        if (!car_state.checkered) {
            return false;
        }
    }
    return true;
}

}  // namespace race