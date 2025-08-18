#pragma once
#include "metric_accumulator.hpp"
#include <unistd.h>

namespace analyser::metric_accumulator::metric_accumulator_impl {

struct SumAverageAccumulator final : IAccumulator {
    struct SumAverage {
        int sum;
        double average;
        auto operator<=>(const SumAverage &) const = default;
    };
    void Accumulate(const metric::MetricResult &metric_result) override;

    void Finalize() override;

    void Reset() override;

    SumAverage Get() const;

private:
    int sum = 0;
    int count = 0;
    double average = 0;
};

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
