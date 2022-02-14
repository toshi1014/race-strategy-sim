#include <iostream>

#include "config.hpp"
#include "tire.hpp"

namespace car {

Tire::Tire(const std::string& str_tire) {
    if (str_tire == "hard")
        compound = Compound::hard;
    else if (str_tire == "medium")
        compound = Compound::medium;
    else if (str_tire == "soft")
        compound = Compound::soft;
    else {
        std::cerr << "Unknown tire compound " << str_tire << std::endl;
        std::exit(1);
    }

    const auto& tire_spec = config::car::tire_spec_dict.at(str_tire);
    lifespan = tire_spec.lifespan;
    base_performance = tire_spec.base_performance;
}

void Tire::step() {
    // COMBAK: tire model
    performance = base_performance * (lifespan - age) / lifespan;
}

}  // namespace car