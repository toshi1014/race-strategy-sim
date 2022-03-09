#include "circuit.hpp"

namespace circuit {

Circuit::Circuit(const std::uint32_t& course_length_,
                 const std::uint32_t& pit_time_loss_)
    : course_length(course_length_), pit_time_loss(pit_time_loss_) {}

}  // namespace circuit