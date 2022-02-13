#include "race.hpp"

namespace race {

Race::Race(circuit::Circuit&& circuit_, const std::uint32_t& num_of_laps_)
    : circuit(circuit_), num_of_laps(num_of_laps_) {}

void Race::add_car(car::Car&& arg_car) { car_list.push_back(arg_car); }

void Race::show_standings() const {
    std::uint32_t idx{};

    std::cout << "pos.\tcar num\t" << std::endl;

    for (const auto& car : car_list) {
        std::cout << ++idx << "\t" << car.car_num << std::endl;
    }
}

}  // namespace race