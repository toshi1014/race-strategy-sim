#include <iostream>
#include <tuple>
#include <vector>

#include "car.hpp"
#include "config.hpp"
#include "race.hpp"

// FIXME: param
constexpr std::uint32_t COURSE_LENGTH{100};
constexpr std::uint32_t NUM_OF_LAPS{50};

int main() {
    race::Race race{circuit::Circuit{COURSE_LENGTH}, NUM_OF_LAPS};

    // {car_num, speed, tire_strategy}, ...
    const std::vector<
        std::tuple<std::uint32_t, double, std::vector<car::TireCompound>>>
        car_data{
            {1, 1., {car::TireCompound::medium, car::TireCompound::hard}},
            {2, 1., {car::TireCompound::hard, car::TireCompound::medium}},
            {3, 1., {car::TireCompound::medium, car::TireCompound::hard}},
            {4, 1., {car::TireCompound::hard, car::TireCompound::medium}},
            {5, 1., {car::TireCompound::medium, car::TireCompound::hard}},
        };

    for (auto& car_datum : car_data) {
        std::uint32_t car_num_now{};
        double speed_now{};
        std::vector<car::TireCompound> tire_strategy{};
        std::tie(car_num_now, speed_now, tire_strategy) = car_datum;
        race.add_car(car::Car{speed_now, car_num_now, tire_strategy});
    }

    race.formation_lap();
    race.show_standings();

    std::cout << "\nstart\n" << std::endl;
    race.start();
}