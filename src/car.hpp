#ifndef CAR_HPP
#define CAR_HPP

#include <cstdint>

namespace car {

class Car {
   private:

   public:
    const std::uint32_t car_num;
    const double speed;  // e.g. 1, 0.75
    Car(const std::uint32_t&, const double&);
};

}  // namespace car

#endif