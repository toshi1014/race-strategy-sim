#include <iostream>

#include "config.hpp"
#include "tire.hpp"

namespace car {

Tire::Tire(const TireCompound& compound_)
    : base_performance(config::TIRE_SPEC_DICT.at(compound_).base_performance),
      lifespan(config::TIRE_SPEC_DICT.at(compound_).lifespan),
      compound(compound_) {
    std::cout << "Tire constructed" << std::endl;
}

Tire::Tire(const Tire& other)
    : base_performance(
          config::TIRE_SPEC_DICT.at(other.compound).base_performance),
      lifespan(config::TIRE_SPEC_DICT.at(other.compound).lifespan),
      compound(other.compound) {
    std::cout << "Tire copied" << std::endl;
}

Tire& Tire::operator=(const Tire& other) {
    std::cout << "Tire::operator=" << std::endl;
    return *this;
}

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
    else if (compound == TireCompound::None) {
        std::cerr << "No tire" << std::endl;
        std::exit(1);
    } else {
        std::cerr << "Unknown compound" << std::endl;
        std::exit(1);
    }
}

TireStrategy::TireStrategy(
    const std::vector<std::tuple<std::string, std::uint32_t>>&
        tire_strategy_str) {
    std::cout << "TireStrategy constructor" << std::endl;

    std::uint32_t tirelap_idx{};
    for (const auto& tirelap_str : tire_strategy_str) {
        std::shared_ptr<Tire> tire_ptr_tmp;
        if (std::get<0>(tirelap_str) == "hard")
            tire_ptr_tmp.reset(new Tire(TireCompound::hard));
        else if (std::get<0>(tirelap_str) == "medium")
            tire_ptr_tmp.reset(new Tire(TireCompound::medium));
        else if (std::get<0>(tirelap_str) == "soft")
            tire_ptr_tmp.reset(new Tire(TireCompound::soft));
        else {
            std::cout << "Unknown compound " << std::get<0>(tirelap_str)
                      << std::endl;
            std::exit(1);
        }

        std::shared_ptr<TireLap> tirelap_ptr_tmp(
            new TireLap{tire_ptr_tmp, std::get<1>(tirelap_str)});

        tire_strategy[tirelap_idx++] = std::move(tirelap_ptr_tmp);
        tirelap_ptr_tmp.reset();
    }

    tire_num = tirelap_idx;
}

TireStrategy::TireStrategy(const TireStrategy& other)
    : tire_num(other.tire_num), tire_strategy(std::move(other.tire_strategy)) {
    std::cout << "TireStrategy copied" << std::endl;
}

// TODO: implement destructor
TireStrategy::~TireStrategy() {
    std::cout << "TireStrategy destructor" << std::endl;
}

const std::shared_ptr<TireLap> TireStrategy::at(
    const std::uint32_t& idx) const {
    if (idx >= tire_num) {
        std::cerr << "Out of Range in TireStrategy" << std::endl;
        std::exit(1);
    }
    return tire_strategy[idx];
}

void TireStrategy::repr() const {
    std::cout << tire_num << " Stop" << std::endl;
    for (std::uint32_t idx{}; idx < tire_num; idx++) {
        std::cout << at(idx)->tire_ptr->get_compound_str() << "\t"
                  << at(idx)->lap << std::endl;
    }
}

}  // namespace car