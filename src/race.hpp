#ifndef RACE_HPP
#define RACE_HPP

#include <cstdint>
#include <iostream>
#include <vector>

#include "car.hpp"
#include "circuit.hpp"

namespace race {

class Race {
   private:
    circuit::Circuit circuit;
    const std::uint32_t num_of_laps;
    std::vector<car::Car> car_list;


   public:
    Race(circuit::Circuit&&, const std::uint32_t&);

    void add_car(car::Car&&);
    void show_standings() const;
};

}  // namespace race

#endif