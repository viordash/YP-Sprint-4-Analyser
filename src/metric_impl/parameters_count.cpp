#include "metric_impl/parameters_count.hpp"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>

namespace analyser::metric::metric_impl {

namespace {
size_t GetParamsBlockIndent(std::string_view params) {
    auto end_line = params.find('\n');
    if (end_line == std::string_view::npos) {
        throw std::invalid_argument{"Invalid AST, params w/o end of line"};
    }
    end_line += sizeof('\n');

    size_t indent = 0;
    auto it = params.begin() + end_line;
    while (*it == ' ' && it != params.end()) {
        it++;
        indent++;
    }
    return indent;
}
}  // namespace

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

    auto params_end_line = params.find('\n');
    if (params_end_line == std::string::npos) {
        throw std::invalid_argument{"Invalid AST, no end of line"};
    }

    auto level_indent = GetParamsBlockIndent(params);
    const std::string param_id = std::format("\n{:{}}(", "", level_indent);
    size_t params_count = 0;
    size_t pos = 0;
    while ((pos = params.find(param_id, pos)) != std::string_view::npos) {
        pos += param_id.length();
        params_count++;
    }
    return params_count;
}

}  // namespace analyser::metric::metric_impl
