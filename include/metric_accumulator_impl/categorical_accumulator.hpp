#pragma once
#include "metric_accumulator.hpp"
#include <unistd.h>

namespace analyser::metric_accumulator::metric_accumulator_impl {

struct CategoricalAccumulator final : IAccumulator {
    void Accumulate(const metric::MetricResult &metric_result) override;

    void Finalize() override;

    void Reset() override;

    const std::unordered_map<int, int> &Get() const;

private:
    std::unordered_map<int, int> categories_freq;
};

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
