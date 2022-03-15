#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <tuple>
#include <vector>

#include "car.hpp"
#include "config.hpp"
#include "race.hpp"

int main() {
    using json = nlohmann::json;
    std::ifstream settings_file("settings.json");
    json settings_json;
    settings_file >> settings_json;

    race::Race race{circuit::Circuit{settings_json["course_length"],
                                     settings_json["pit_time_loss"]},
                    settings_json["num_of_laps"]};

    for (const auto& car_datum : settings_json["car_data"]) {
        std::vector<std::tuple<std::string, std::uint32_t>> raw_tire_strategy;

        for (const auto& strategy : car_datum["tire_strategy"]) {
            raw_tire_strategy.push_back(
                {strategy["compound"].get<std::string>(),
                 strategy["lap"].get<std::uint32_t>()});
        }

        race.add_car(car::Car{car_datum["speed"].get<double>(),
                              car_datum["car_num"].get<std::uint32_t>(),
                              {raw_tire_strategy}});
    }

    race.formation_lap();
    race.show_standings();

    std::cout << "\n\n========= Start =========\n" << std::endl;

    race.start();

    race.show_standings();
}