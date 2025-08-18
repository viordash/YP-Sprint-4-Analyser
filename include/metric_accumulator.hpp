#pragma once
#include "metric.hpp"
#include <algorithm>
#include <string>
#include <unistd.h>
#include <vector>

namespace rv = std::ranges::views;
namespace rs = std::ranges;

namespace analyser::metric_accumulator {

struct IAccumulator {
    virtual void Accumulate(const metric::MetricResult &metric_result) = 0;
    virtual void Finalize() = 0;
    virtual void Reset() = 0;
    virtual ~IAccumulator() = default;

protected:
    bool is_finalized = false;
};

struct MetricsAccumulator {
    template <typename Accumulator>
    void RegisterAccumulator(const std::string &metric_name, std::unique_ptr<Accumulator> acc) {
        accumulators[metric_name] = std::move(acc);
    }

    template <typename Accumulator>
    const Accumulator &GetFinalizedAccumulator(const std::string &metric_name) const {
        auto it = accumulators.find(metric_name);
        if (it == accumulators.end()) {
            throw std::invalid_argument("No accumulator for metric: " + metric_name);
        }

        auto ptr = std::dynamic_pointer_cast<Accumulator>(it->second);
        if (ptr == nullptr) {
            throw std::runtime_error("Accumulator for " + metric_name + " has incorrect type");
        }
        ptr->Finalize();
        return *ptr;
    }

    void AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const;
    void ResetAccumulators();

private:
    std::unordered_map<std::string, std::shared_ptr<IAccumulator>> accumulators;
};

}  // namespace analyser::metric_accumulator
