#include <tuple>
#include <vector>

#include "car.hpp"
#include "race.hpp"

// FIXME: param
constexpr std::uint32_t COURSE_LENGTH = 100;
constexpr std::uint32_t NUM_OF_LAPS = 50;

int main() {
    race::Race race{circuit::Circuit{COURSE_LENGTH}, NUM_OF_LAPS};

    // {car_num, speed}, ...
    std::vector<std::tuple<std::uint32_t, double>> car_data = {
        {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1},
    };

    for (auto& car_datum : car_data) {
        std::uint32_t car_num_now {};
        double speed_now {};
        std::tie(car_num_now, speed_now) = car_datum;
        race.add_car(car::Car{car_num_now, speed_now});
    }
    race.show_standings();
}