#include <iostream>

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
}

void Tire::step(){
}

}  // namespace car