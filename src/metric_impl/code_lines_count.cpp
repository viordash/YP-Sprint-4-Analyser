#include "metric_impl/code_lines_count.hpp"
#include "utils.hpp"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <unistd.h>
#include <vector>

namespace analyser::metric::metric_impl {

std::string CodeLinesCountMetric::Name() const { return "CodeLinesCount"; }

namespace {
std::tuple<int, size_t> get_line_number(std::string_view ast) {
    auto start_pos = ast.find('[');
    if (start_pos == std::string::npos) {
        throw std::invalid_argument{"Invalid AST, not found '['"};
    }

    auto end_pos = ast.find(']', start_pos);
    if (end_pos == std::string::npos) {
        throw std::invalid_argument{"Invalid AST, not found ']'"};
    }

    std::string_view coord_sv(ast.data() + start_pos + 1, end_pos - start_pos - 1);
    size_t comma_pos = coord_sv.find(',');
    if (comma_pos == std::string_view::npos) {
        throw std::invalid_argument{"Invalid AST, not found comma"};
    }
    std::string_view start_sv(coord_sv.data(), comma_pos);
    int line_number = ToInt(start_sv.substr(0, comma_pos));

    return {line_number, end_pos};
}
}  // namespace

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &f) const {
    auto lines = f.ast | std::views::split('\n')                                        //
                 | std::views::transform([](auto &&r) { return std::string_view{r}; })  //
                 | std::ranges::to<std::vector>();
    if (lines.size() < 1) {
        throw std::invalid_argument{"Invalid AST, empty string"};
    }

    auto function_definition = lines[0];
    auto [start_line, start_pos] = get_line_number(function_definition);

    std::string_view rest(function_definition.begin() + start_pos, function_definition.end());
    auto [end_line, end_pos] = get_line_number(rest);
    auto count = end_line - start_line + 1;

    auto comments = lines | std::views::filter([](auto &&str) { return str.contains("(comment"); });
    auto comments_count = std::ranges::distance(comments.begin(), comments.end());

    return count - comments_count;
}

}  // namespace analyser::metric::metric_impl
