#include "metric_accumulator.hpp"
#include <algorithm>
#include <ranges>
#include <unistd.h>
#include <vector>

namespace analyser::metric_accumulator {

void MetricsAccumulator::AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const {
    rs::for_each(metric_results, [&](const auto &result) {
        auto it = accumulators.find(result.metric_name);
        if (it == accumulators.end()) {
            throw std::invalid_argument("No accumulator for metric: " + result.metric_name);
        }
        it->second->Accumulate(result);
    });
}

void MetricsAccumulator::ResetAccumulators() {
    rs::for_each(accumulators | rv::values, [](const auto &acc_ptr) {
        if (acc_ptr == nullptr) {
            throw std::runtime_error("Accumulator missed");
        }
        acc_ptr->Reset();
    });
}

}  // namespace analyser::metric_accumulator
