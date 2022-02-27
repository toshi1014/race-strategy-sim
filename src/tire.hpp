#ifndef TIRE
#define TIRE

#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace car {

constexpr std::uint32_t MAX_TIRE_NUM = 10;

enum struct TireCompound {
    hard,
    medium,
    soft,
    None,
};

class Tire {
   private:
    double performance{1.};

    const double base_performance;

    const std::uint32_t lifespan;

   public:
    const TireCompound compound{TireCompound::None};

    std::uint32_t age{1};

    explicit Tire(const TireCompound&);

    Tire(const Tire&);

    Tire& operator=(const Tire&);

    void step();

    void next_lap();

    std::string get_compound_str() const;

    friend class Car;
};

struct TireLap {
    std::shared_ptr<Tire> tire_ptr;
    const std::uint32_t& lap;
};

struct TireStrategy {
   private:
    std::uint32_t tire_num;

   public:
    std::shared_ptr<TireLap> tire_strategy[MAX_TIRE_NUM];

    TireStrategy(const std::vector<std::tuple<std::string, std::uint32_t>>&);

    TireStrategy(const TireStrategy& other);

    ~TireStrategy();

    const std::shared_ptr<TireLap> at(const std::uint32_t&) const;

    std::uint32_t size() const;

    void repr() const;
};

}  // namespace car

#endif