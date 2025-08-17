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
        parameters: (parameters 
          (identifier))
        body: (block)))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 1);
}

TEST(CountParametersMetricTests, multiple_mixed_parameters) {
    CountParametersMetric metric;
    auto func = function::Function{.ast = R"((function_definition [3, 4] - [5, 28]
        name: (identifier [3, 8] - [3, 16])
        parameters: (parameters [3, 16] - [3, 68]
          (identifier [3, 17] - [3, 21])
          (typed_default_parameter [3, 23] - [3, 46]
            name: (identifier [3, 23] - [3, 33])
            type: (type [3, 35] - [3, 40]
              (identifier [3, 35] - [3, 40]))
            value: (float [3, 43] - [3, 46]))
          (typed_default_parameter [3, 48] - [3, 67]
            name: (identifier [3, 48] - [3, 54])
            type: (type [3, 56] - [3, 61]
              (identifier [3, 56] - [3, 61]))
            value: (float [3, 64] - [3, 67])))
        body: (block [4, 8] - [5, 28])"};

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

class CountParametersFromTestFiles : public ::testing::TestWithParam<std::tuple<std::string, int>> {};

TEST_P(CountParametersFromTestFiles, from_test_file) {
    auto filename = std::get<0>(GetParam());
    auto count = std::get<1>(GetParam());

    CountParametersMetric metric;
    function::FunctionExtractor func_extractor;
    auto file = file::File(std::get<0>(GetParam()));

    auto func = func_extractor.Get(file);
    ASSERT_EQ(func.size(), 1);
    auto result = metric.Calculate(func[0]);
    ASSERT_EQ(result.value, std::get<1>(GetParam()));
}

const static std::string filepath = "../src/metric_impl/tests/files/";
INSTANTIATE_TEST_SUITE_P(_, CountParametersFromTestFiles,
                         ::testing::Values(                                        //
                             std::make_tuple(filepath + "comments.py", 3),         //
                             std::make_tuple(filepath + "exceptions.py", 0),       //
                             std::make_tuple(filepath + "if.py", 1),               //
                             std::make_tuple(filepath + "loops.py", 1),            //
                             std::make_tuple(filepath + "many_lines.py", 0),       //
                             std::make_tuple(filepath + "many_parameters.py", 5),  //
                             std::make_tuple(filepath + "match_case.py", 1),       //
                             std::make_tuple(filepath + "nested_if.py", 2),        //
                             std::make_tuple(filepath + "simple.py", 0),           //
                             std::make_tuple(filepath + "ternary.py", 1)           //
                             ));

}  // namespace analyser::metric::metric_impl
