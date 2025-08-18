#include "metric_impl/cyclomatic_complexity.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ranges>
#include <string>
#include <string_view>
#include <unistd.h>
#include <vector>

namespace analyser::metric::metric_impl {
std::string CyclomaticComplexityMetric::Name() const { return "CyclomaticComplexity"; }

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {
    static const std::array<std::string_view, 9> constructions = {
        "(if_statement",   "(elif_clause",  "(while_statement",  "(for_statement",         "(except_clause",
        "(finally_clause", "(case_pattern", "(assert_statement", "(conditional_expression"};

    auto lines = f.ast | std::views::split('\n')                                        //
                 | std::views::transform([](auto &&r) { return std::string_view{r}; })  //
                 | std::ranges::to<std::vector>();
    if (lines.size() < 1) {
        throw std::invalid_argument{"Invalid AST, empty string"};
    }

    auto complexity = std::ranges::count_if(lines, [&](const std::string_view line) {
        return std::ranges::any_of(constructions, [&](const std::string_view constr) { return line.contains(constr); });
    });

    const size_t base_complexity = 1;
    return base_complexity + complexity;
}

}  // namespace analyser::metric::metric_impl
