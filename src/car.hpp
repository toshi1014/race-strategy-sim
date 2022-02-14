#ifndef CAR_HPP
#define CAR_HPP

#include <cstdint>
#include <string>

#include "tire.hpp"

namespace car {

class Car {
   private:
    const double speed;  // e.g. 1, 0.75

   public:
    const std::uint32_t car_num;
    std::uint32_t lap;

    Tire tire;

    Car(const double&, const std::uint32_t&, const std::string&);

    double step();
};

}  // namespace car

#endif
