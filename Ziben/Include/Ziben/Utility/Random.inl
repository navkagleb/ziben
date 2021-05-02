namespace Ziben {

    template <BoolConcept T>
    T Random_Impl::Get(float probability) {
        return BooleanDistribution(probability)(m_MersenneTwister);
    }

    template <IntegralConcept T>
    T Random_Impl::Get(T left, T right) {
        return IntegerDistribution<T>(std::min(left, right), std::max(left, right))(m_MersenneTwister);
    }

    template <FloatingPointConcept T>
    T Random_Impl::Get(T left, T right) {
        return RealDistribution<T>(std::min(left, right), std::max(left, right))(m_MersenneTwister);
    }

} // namespace Ziben