#include "metric_accumulator_impl/sum_average_accumulator.hpp"
#include <unistd.h>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void SumAverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    if (is_finalized) {
        throw std::logic_error("Already finalized");
    }
    sum += metric_result.value;
    count++;
}

void SumAverageAccumulator::Finalize() {
    if (count > 0) {
        average = static_cast<double>(sum) / count;
    } else {
        average = 0.0;
    }
    is_finalized = true;
}

void SumAverageAccumulator::Reset() {
    sum = 0;
    count = 0;
    average = 0.0;
    is_finalized = false;
}

SumAverageAccumulator::SumAverage SumAverageAccumulator::Get() const {
    if (!is_finalized) {
        throw std::logic_error("Not finalized");
    }
    return {.sum = sum, .average = average};
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
