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
};

class Race {
   private:
    circuit::Circuit circuit;

    const std::uint32_t num_of_laps;

    std::vector<CarState> car_state_list;

    bool grid_fixed{false};

   public:
    Race(circuit::Circuit&&, const std::uint32_t&);

    void add_car(car::Car&&);

    void formation_lap();

    void show_standings() const;

    void start();
    
    void step();
};

}  // namespace race

#endif