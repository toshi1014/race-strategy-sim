#ifndef CAR_HPP
#define CAR_HPP

#include <cstdint>
#include <string>

#include "tire.hpp"

namespace car {

class Car {
   private:
   public:
    const std::uint32_t car_num;

    const double speed;  // e.g. 1, 0.75

    std::uint32_t lap{1};

    Tire tire;

    Car(const std::uint32_t&, const double&, const std::string&);

    void step();
};

}  // namespace car

#endif