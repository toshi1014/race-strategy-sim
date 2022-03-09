#include <iostream>
#include <tuple>
#include <vector>

#include "car.hpp"
#include "config.hpp"
#include "race.hpp"

// FIXME: param
constexpr std::uint32_t COURSE_LENGTH{10};
constexpr std::uint32_t PIT_TIME_LOSS{1};
constexpr std::uint32_t NUM_OF_LAPS{20};

int main() {
    race::Race race{circuit::Circuit{COURSE_LENGTH, PIT_TIME_LOSS}, NUM_OF_LAPS};

    // {car_num, speed, tire_strategy}, ...
    std::vector<std::tuple<std::uint32_t, double,
                           std::vector<std::tuple<std::string, std::uint32_t>>>>
        car_data{
            {1,
             1.,
             {{
                 {"medium", 10},
                 {"hard", 21},
             }}},
            {2,
             1.,
             {{
                 {"hard", 10},
                 {"medium", 21},
             }}},
            {3,
             1.,
             {{
                 {"medium", 10},
                 {"hard", 21},
             }}},
            {4,
             1.,
             {{
                 {"hard", 10},
                 {"medium", 21},
             }}},
            {5,
             1.,
             {{
                 {"medium", 10},
                 {"hard", 21},
             }}},
        };

    for (const auto& car_datum : car_data) {
        const std::uint32_t car_num_now{std::get<0>(car_datum)};
        const double speed_now{std::get<1>(car_datum)};
        const car::TireStrategy tire_strategy{std::get<2>(car_datum)};
        race.add_car(car::Car{speed_now, car_num_now, tire_strategy});
    }

    race.formation_lap();
    race.show_standings();

    std::cout << "\n\n========= Start =========\n" << std::endl;

    race.start();

    race.show_standings();
}