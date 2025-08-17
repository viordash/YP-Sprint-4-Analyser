#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

TEST(CountParametersMetricTests, get_metric_name) {
    CountParametersMetric metric;
    auto func = function::Function{.ast = R"((function_definition
        name: (identifier)
        parameters: (parameters)
        body: (block)))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.metric_name, "ParametersCount");
}

TEST(CountParametersMetricTests, no_parameters) {
    CountParametersMetric metric;
    auto func = function::Function{.ast = R"((function_definition
        name: (identifier)
        parameters: (parameters)
        body: (block)))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 0);
}

TEST(CountParametersMetricTests, one_simple_parameter) {
    CountParametersMetric metric;
    auto func = function::Function{.ast = R"((function_definition
        parameters: (parameters (identifier))
        body: (block)))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 1);
}

TEST(CountParametersMetricTests, multiple_mixed_parameters) {
    CountParametersMetric metric;
    auto func = function::Function{.ast = R"((function_definition
        parameters: (parameters
            (identifier)
            (typed_parameter (identifier) type: (type))
            (typed_default_parameter (identifier) type: (type) value: (float)))
        body: (block)))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 3);
}

TEST(CountParametersMetricTests, args_and_kwargs) {
    CountParametersMetric metric;
    auto func = function::Function{.ast = R"((function_definition
        parameters: (parameters
            (identifier)
            (default_parameter (identifier) value: (integer))
            (list_splat_pattern (identifier))
            (dictionary_splat_pattern (identifier)))
        body: (block)))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 4);
}

}  // namespace analyser::metric::metric_impl
