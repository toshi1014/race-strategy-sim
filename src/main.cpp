#include <iostream>
#include <tuple>
#include <vector>

#include "car.hpp"
#include "config.hpp"
#include "race.hpp"

// FIXME: param
constexpr std::uint32_t COURSE_LENGTH{10};
constexpr std::uint32_t NUM_OF_LAPS{20};

int main() {
    race::Race race{circuit::Circuit{COURSE_LENGTH}, NUM_OF_LAPS};

    // {car_num, speed, tire_strategy}, ...
    std::vector<std::tuple<std::uint32_t, double, car::TireStrategy>> car_data{
        {1,
         1.,
         {{
             {car::TireCompound::medium, 10},
             {car::TireCompound::hard, 20},
         }}},
        {2,
         2.,
         {{
             {car::TireCompound::hard, 10},
             {car::TireCompound::medium, 20},
         }}},
        {3,
         1.,
         {{
             {car::TireCompound::medium, 10},
             {car::TireCompound::hard, 20},
         }}},
        {4,
         1.,
         {{
             {car::TireCompound::hard, 10},
             {car::TireCompound::medium, 20},
         }}},
        {5,
         1.,
         {{
             {car::TireCompound::medium, 10},
             {car::TireCompound::hard, 20},
         }}},
    };

    for (const auto& car_datum : car_data) {
        std::uint32_t car_num_now{std::get<0>(car_datum)};
        double speed_now{std::get<1>(car_datum)};
        car::TireStrategy tire_strategy{std::get<2>(car_datum)};
        race.add_car(car::Car{speed_now, car_num_now, tire_strategy});
    }

    race.formation_lap();
    race.show_standings();

    std::cout << "\n\n========= Start =========\n" << std::endl;

    race.start();

    race.show_standings();
}