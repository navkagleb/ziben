#pragma once

#include "Singleton.hpp"

namespace Ziben {

    template <typename T>
    concept BoolConcept = std::is_same_v<bool, T>;

    template <typename T>
    concept IntegralConcept = std::is_integral_v<T>;

    template <typename T>
    concept FloatingPointConcept = std::is_floating_point_v<T>;

    class Random_Impl {
    public:
        friend class Singleton<Random_Impl>;

    public:
                              using BooleanDistribution = std::bernoulli_distribution;
        template <typename T> using IntegerDistribution = std::uniform_int_distribution<T>;
        template <typename T> using RealDistribution    = std::uniform_real_distribution<T>;
        template <typename T> using Limit               = std::numeric_limits<T>;

    public:
        Random_Impl();
        ~Random_Impl() = default;

    public:
        template <BoolConcept T>
        T Get(float probability = 0.5f);

        template <IntegralConcept T>
        inline T Get(T left = Limit<T>::min(), T right = Limit<T>::max());

        template <FloatingPointConcept T>
        T Get(T left = Limit<T>::min(), T right = Limit<T>::max());

    private:
        std::random_device m_RandomDevice;
        std::seed_seq      m_SeedSequence;
        std::mt19937       m_MersenneTwister;

    }; // class Random_Impl

    using Random = Singleton<Random_Impl>;

} // namespace Ziben

#include "Random.inl"