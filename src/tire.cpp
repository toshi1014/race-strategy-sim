#include <iostream>

#include "config.hpp"
#include "tire.hpp"

namespace car {

Tire::Tire(const TireCompound& compound_)
    : base_performance(config::TIRE_SPEC_DICT.at(compound_).base_performance),
      lifespan(config::TIRE_SPEC_DICT.at(compound_).lifespan),
      compound(compound_) {}

void Tire::step() {
    // COMBAK: tire model
    performance = base_performance * (lifespan - age) / lifespan;
}

void Tire::next_lap() { age++; }

std::string Tire::get_compound_str() const {
    if (compound == TireCompound::hard)
        return "H";
    else if (compound == TireCompound::medium)
        return "M";
    else if (compound == TireCompound::soft)
        return "S";
    else {
        std::cerr << "Unknown compound";
        std::exit(1);
    }
}

}  // namespace car