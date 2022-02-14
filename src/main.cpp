#include <iostream>
#include <tuple>
#include <vector>

#include "car.hpp"
#include "race.hpp"

// FIXME: param
constexpr std::uint32_t COURSE_LENGTH{100};
constexpr std::uint32_t NUM_OF_LAPS{50};

int main() {
    race::Race race{circuit::Circuit{COURSE_LENGTH}, NUM_OF_LAPS};

    // {car_num, speed}, ...
    const std::vector<std::tuple<std::uint32_t, double, std::string>> car_data =
        {
            {1, 1, "medium"}, {2, 1, "medium"}, {3, 1, "medium"},
            {4, 1, "medium"}, {5, 1, "medium"},
        };

    for (auto& car_datum : car_data) {
        std::uint32_t car_num_now{};
        double speed_now{};
        std::string tire_now{};
        std::tie(car_num_now, speed_now, tire_now) = car_datum;
        race.add_car(car::Car{speed_now, car_num_now, tire_now});
    }

    race.formation_lap();
    race.show_standings();

    std::cout << "\nstart\n" << std::endl;
    race.start();
}