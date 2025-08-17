#include "metric.hpp"
#include <string>

namespace analyser::metric::metric_impl {

struct CountParametersMetric final : IMetric {
    MetricResult::ValueType CalculateImpl(const function::Function &f) const override;
    std::string Name() const override;
};

}  // namespace analyser::metric::metric_impl
