#ifndef RACE_HPP
#define RACE_HPP

#include <cstdint>
#include <iostream>
#include <vector>

#include "car.hpp"
#include "circuit.hpp"
#include "config.hpp"

namespace race {

struct CarState {
    std::uint32_t position;
    double distance;  // distance from start line
    car::Car car;

    CarState& operator=(const CarState&);
};

class Race {
   private:
    circuit::Circuit circuit;

    const std::uint32_t num_of_laps;

    std::vector<CarState> car_state_list;

    bool grid_fixed{false};

    bool bool_checkered{false};

    void next_lap(CarState&);

    void step();

    bool is_checkered(const CarState&) const;

    CarState& get_forerunner(const CarState&);

    double get_distance_gap(const CarState&, const CarState&) const;

    void overtake(CarState&, CarState&);

    const CarState& get_car_state_by_position(const std::uint32_t&) const;

   public:
    Race(circuit::Circuit&&, const std::uint32_t&);

    void add_car(car::Car&&);

    void formation_lap();

    void show_standings() const;

    void start();
};

}  // namespace race

#endif