#include "metric_impl/parameters_count.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>

namespace analyser::metric::metric_impl {

std::string CountParametersMetric::Name() const { return "ParametersCount"; }

MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function &f) const {
    auto params_start = f.ast.find("(parameters");
    if (params_start == std::string::npos) {
        return 0;
    }

    auto params_end = f.ast.find("body: (block", params_start);
    if (params_end == std::string::npos) {
        throw std::invalid_argument{"Invalid AST, not found '(block'"};
    }

    std::string_view params(f.ast.data() + params_start + 1, params_end - params_start - 1);

    const std::string_view identifier = "(identifier";
    size_t params_count = 0;
    size_t pos = 0;
    while ((pos = params.find(identifier, pos)) != std::string_view::npos) {
        pos += identifier.length();
        params_count++;
    }
    return params_count;
}

}  // namespace analyser::metric::metric_impl
