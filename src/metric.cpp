#include "metric.hpp"
#include "function.hpp"
#include <ranges>
#include <stdexcept>
#include <unistd.h>
#include <vector>

namespace analyser::metric {

void MetricExtractor::RegisterMetric(std::unique_ptr<IMetric> metric) {
    if (metric == nullptr) {
        throw std::invalid_argument{"metric is null"};
    }
    metrics.push_back(std::move(metric));
}

MetricResults MetricExtractor::Get(const function::Function &func) const {
    auto calculated = metrics | std::views::transform([&](const auto &metric) { return metric->Calculate(func); });
    return MetricResults(calculated.begin(), calculated.end());
}

}  // namespace analyser::metric
