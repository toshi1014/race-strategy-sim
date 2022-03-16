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
        std::uint32_t position_now = car_state_ptr_list.size() + 1;
        CarStatePtr car_state_ptr(new CarState{
            position_now, -static_cast<double>(position_now * config::GRID_GAP),
            arg_car,
            false,  // checkered
            false,  // in_pit
            0,      // pit_time_loss
        });
        std::cout << "car adding..." << std::endl;
        car_state_ptr_list.push_back(std::move(car_state_ptr));
        std::cout << "car added" << std::endl;
    } else {
        std::cerr << "Grid fixed, no more cars" << std::endl;
        std::exit(1);
    }
}

const CarStatePtr& Race::get_car_state_by_position(
    const std::uint32_t& position) const {
    const auto car_state_it = std::find_if(
        std::begin(car_state_ptr_list), std::end(car_state_ptr_list),
        [&](const CarStatePtr car_state_ptr) {
            return car_state_ptr->position == position;
        });

    return *car_state_it;
}

const CarStatePtr& Race::get_car_state_by_car_num(
    const std::uint32_t& car_num) const {
    const auto car_state_it = std::find_if(
        std::begin(car_state_ptr_list), std::end(car_state_ptr_list),
        [&](const CarStatePtr car_state_ptr) {
            return car_state_ptr->car.car_num == car_num;
        });

    return *car_state_it;
}

void Race::show_standings() const {
    std::cout << "Pos.\tNo.\tLap\tTire\tAge\n" << std::endl;

    for (std::uint32_t pos_i{1}; pos_i <= car_state_ptr_list.size(); pos_i++) {
        const CarStatePtr car_state_ptr =
            Race::get_car_state_by_position(pos_i);
        std::cout << pos_i << "\t" << car_state_ptr->car.car_num << "\t"
                  << car_state_ptr->car.lap << "\t"
                  << car_state_ptr->car.tire_ptr->get_compound_str() << "\t"
                  << car_state_ptr->car.tire_ptr->age << "\t" << std::endl;
    }
}

void Race::formation_lap() { grid_fixed = true; }

void Race::start() {
    for (double timer{}; timer < 30. && !all_checkered; timer += config::TICK) {
        step();

        // {
        //     std::cout << "\n\ntime:\t" << timer << "\n" << std::endl;
        //     for (const auto& car_state_ptr : car_state_ptr_list) {
        //         std::cout << "No." << car_state_ptr->car.car_num
        //                   << "\tlap: " << car_state_ptr->car.lap
        //                   << "\tdis: " << car_state_ptr->distance << "\tPos."
        //                   << car_state_ptr->position << std::endl;
        //     }
        //     std::cout << "" << std::endl;
        // }
    }

    std::cout << "\n\n========= Checkered =========\n" << std::endl;
}

void Race::next_lap(CarStatePtr& car_state_ptr) {
    if (Race::is_checkered(car_state_ptr)) {
        std::cout << "No." << car_state_ptr->car.car_num << "\tcheckered"
                  << std::endl;
        checker_flg = true;
        car_state_ptr->checkered = true;
        all_checkered = Race::is_all_checkered();
    } else {
        car_state_ptr->distance =
            car_state_ptr->distance - circuit.course_length;
        bool pit_stop = car_state_ptr->car.next_lap();
        if (pit_stop) Race::pit_stop(car_state_ptr);

        std::cout << "No." << car_state_ptr->car.car_num
                  << "\tnext lap: " << car_state_ptr->car.lap << std::endl;
    }
}

void Race::step() {
    for (std::uint32_t pos_i{1}; pos_i <= car_state_ptr_list.size(); pos_i++) {
        CarStatePtr car_state_ptr = Race::get_car_state_by_position(pos_i);

        if (car_state_ptr->checkered) continue;

        double delta{};

        if (pos_i == 1) {
            delta = car_state_ptr->car.step(config::BLOCKABLE_RANGE + 1.,
                                            car_state_ptr->car);

        } else {
            if (car_state_ptr->in_pit) {
                car_state_ptr->pit_time_loss += config::TICK;
                if (car_state_ptr->pit_time_loss >= circuit.pit_time_loss)
                    Race::pit_exit(car_state_ptr);

            } else {
                CarStatePtr forerunner = Race::get_forerunner(car_state_ptr);
                const double distance_gap =
                    Race::get_distance_gap(forerunner, car_state_ptr);

                delta = car_state_ptr->car.step(distance_gap, forerunner->car);

                if (delta >= distance_gap)
                    Race::overtake(forerunner, car_state_ptr);
            }
        }

        if (delta == 0.0) {
            std::cerr << "No." << car_state_ptr->car.car_num << " stopped"
                      << std::endl;
            std::exit(1);
        }

        car_state_ptr->distance += delta;

        if (car_state_ptr->distance >= circuit.course_length)
            Race::next_lap(car_state_ptr);
    }
}

bool Race::is_checkered(const CarStatePtr& car_state_ptr) const {
    return car_state_ptr->car.lap >= num_of_laps || checker_flg;
}

CarStatePtr& Race::get_forerunner(const CarStatePtr& self) {
    const auto it = std::find_if(
        std::begin(car_state_ptr_list), std::end(car_state_ptr_list),
        [&](const CarStatePtr car_state_ptr) {
            return car_state_ptr->position == self->position - 1;
        });
    return *it;
}

double Race::get_distance_gap(const CarStatePtr& forerunner,
                              const CarStatePtr& self) const {
    const double gap_base{std::abs(forerunner->distance - self->distance)};
    const double gap{std::min(gap_base, circuit.course_length - gap_base)};

    return gap + (forerunner->car.lap - self->car.lap) * circuit.course_length;
}

void Race::overtake(CarStatePtr& forerunner, CarStatePtr& overtaking_car) {
    if (!forerunner->checkered) {
        if (forerunner->car.lap == overtaking_car->car.lap) {
            std::cout << "No." << overtaking_car->car.car_num << "\tpassed No."
                      << forerunner->car.car_num << std::endl;

            forerunner->position++;
            overtaking_car->position--;
        }
    }
}

bool Race::is_all_checkered() const {
    for (const auto& car_state_ptr : car_state_ptr_list) {
        if (!car_state_ptr->checkered) {
            return false;
        }
    }
    return true;
}

// static func
void Race::pit_stop(CarStatePtr& car_state_ptr) {
    car_state_ptr->in_pit = true;

    car_state_ptr->car.change_tire();
    std::cout << "No." << car_state_ptr->car.car_num << "\tchanged to "
              << car_state_ptr->car.tire_ptr->get_compound_str() << " tire"
              << std::endl;
}

void Race::pit_exit(CarStatePtr& car_state_ptr) {
    car_state_ptr->in_pit = false;
    car_state_ptr->pit_time_loss = 0.;
}

}  // namespace race