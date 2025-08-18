#include "metric_accumulator_impl/categorical_accumulator.hpp"
#include <unistd.h>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void CategoricalAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    if (is_finalized) {
        throw std::logic_error("Already finalized");
    }
    categories_freq[metric_result.value]++;
}

void CategoricalAccumulator::Finalize() { is_finalized = true; }

void CategoricalAccumulator::Reset() {
    categories_freq.clear();
    is_finalized = false;
}

const std::unordered_map<int, int> &CategoricalAccumulator::Get() const {
    if (!is_finalized) {
        throw std::logic_error("Not finalized");
    }
    return categories_freq;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
