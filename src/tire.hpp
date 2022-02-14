#ifndef TIRE
#define TIRE

#include <string>

namespace car {

class Tire {
   private:
    double performance {1.};
    std::uint32_t age {};
    
    enum struct Compound {
        hard,
        medium,
        soft,
    };

   public:
    Compound compound;

    explicit Tire(const std::string&);

    void step();
};

}  // namespace car

#endif