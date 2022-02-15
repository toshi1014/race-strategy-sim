#include <iostream>

#include "config.hpp"
#include "tire.hpp"

namespace car {

Tire::Tire(const TireCompound& compound_)
    : base_performance(config::tire_spec_dict.at(compound_).base_performance),
      lifespan(config::tire_spec_dict.at(compound_).lifespan),
      compound(compound_) {}

void Tire::step() {
    // COMBAK: tire model
    performance = base_performance * (lifespan - age) / lifespan;
}

void Tire::next_lap() { age++; }

}  // namespace car