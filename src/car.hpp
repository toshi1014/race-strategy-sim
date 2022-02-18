#ifndef CAR_HPP
#define CAR_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "tire.hpp"

namespace car {

class Car {
   private:
    const double speed;  // e.g. 1, 0.75

   public:
    const std::uint32_t car_num;

    std::uint32_t lap{1};

    Tire tire;

    Car(const double&, const std::uint32_t&, const std::vector<TireCompound>&);

    Car(const Car&);

    Car& operator=(const Car&);

    void change_tire(const std::string&);

    double step(const double&, const car::Car&);

    void next_lap();
};

}  // namespace car

#endif