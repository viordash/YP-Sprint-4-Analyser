#include "metric_impl/code_lines_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

TEST(CodeLinesCountMetricTests, get_metric_name) {
    CodeLinesCountMetric metric;
    auto func = function::Function{.ast = "(function_definition [10, 0] - [14, 15] body: (block ...))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.metric_name, "CodeLinesCount");
}

TEST(CodeLinesCountMetricTests, multiline_function) {
    CodeLinesCountMetric metric;
    auto func = function::Function{.ast = "(function_definition [10, 0] - [14, 15] body: (block ...))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 5);
}

TEST(CodeLinesCountMetricTests, singleline_function) {
    CodeLinesCountMetric metric;
    auto func = function::Function{
        .ast = "(function_definition [5, 0] - [5, 22] name: (identifier) parameters: ... body: ...)"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 1);
}

TEST(CodeLinesCountMetricTests, multiline_docstring) {
    CodeLinesCountMetric metric;
    auto func = function::Function{.ast = R"((function_definition [20, 0] - [25, 21]
        name: (identifier)
        parameters: (parameters)
        body: (block
            (expression_statement (string))
            (return_statement (true)))))"};

    auto result = metric.Calculate(func);

    ASSERT_EQ(result.value, 6);
}

TEST(CodeLinesCountMetricTests, comments_are_ignored) {
    CodeLinesCountMetric metric;
    auto func = function::Function{.ast = R"((function_definition [20, 0] - [30, 21]
        name: (identifier)
        parameters: (parameters)
        body: (block
            (comment [22, 4] - [22, 46])
            (expression_statement (string))
            (comment [25, 4] - [25, 54])
            (return_statement (true)))))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 9);
}

TEST(CodeLinesCountMetricTests, malformed_ast_throws_invalid_argument) {
    CodeLinesCountMetric metric;
    auto func = function::Function{.ast = "(function_definition body: (block ...))"};

    EXPECT_THROW(metric.Calculate(func), std::invalid_argument);
}
}  // namespace analyser::metric::metric_impl
