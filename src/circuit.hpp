#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP

#include <cstdint>

namespace circuit {

class Circuit {
   private:
   public:
    const std::uint32_t course_length;
    const std::uint32_t pit_time_loss;

    Circuit(const std::uint32_t&, const std::uint32_t&);
};

}  // namespace circuit

#endif