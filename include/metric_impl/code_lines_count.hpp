#pragma once
#include "metric.hpp"
#include <string>
#include <unistd.h>

namespace analyser::metric::metric_impl {

struct CodeLinesCountMetric final : IMetric {
    MetricResult::ValueType CalculateImpl(const function::Function &f) const override;
    std::string Name() const override;
};

}  // namespace analyser::metric::metric_impl
