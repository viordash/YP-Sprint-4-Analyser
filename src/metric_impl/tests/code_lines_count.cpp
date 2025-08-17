#include "metric_impl/code_lines_count.hpp"

#include <gtest/gtest.h>
#include <string>

namespace analyser::metric::metric_impl {

TEST(CodeLinesCountMetricTests, get_metric_name) {
    CodeLinesCountMetric metric;
    auto func = function::Function{.ast = "(function_definition [10, 0] - [14, 15] body: (block ...))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.metric_name, "CodeLinesCount");
}

TEST(CodeLinesCountMetricTests, singleline_function) {
    CodeLinesCountMetric metric;
    auto func = function::Function{
        .ast = "(function_definition [5, 0] - [5, 22] name: (identifier) parameters: ... body: ...)"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 1);
}

TEST(CodeLinesCountMetricTests, malformed_ast_throws_invalid_argument) {
    CodeLinesCountMetric metric;
    auto func = function::Function{.ast = "(function_definition body: (block ...))"};

    EXPECT_THROW(metric.Calculate(func), std::invalid_argument);
}

class CodeLinesCountFromTestFiles : public ::testing::TestWithParam<std::tuple<std::string, int>> {};

TEST_P(CodeLinesCountFromTestFiles, from_test_file) {
    auto filename = std::get<0>(GetParam());
    auto count = std::get<1>(GetParam());

    CodeLinesCountMetric metric;
    function::FunctionExtractor func_extractor;
    auto file = file::File(std::get<0>(GetParam()));

    auto func = func_extractor.Get(file);
    ASSERT_EQ(func.size(), 1);
    auto result = metric.Calculate(func[0]);
    ASSERT_EQ(result.value, std::get<1>(GetParam()));
}

const static std::string filepath = "../../../src/metric_impl/tests/files/";
INSTANTIATE_TEST_SUITE_P(_, CodeLinesCountFromTestFiles,
                         ::testing::Values(                                        //
                             std::make_tuple(filepath + "comments.py", 4),         //
                             std::make_tuple(filepath + "exceptions.py", 8),       //
                             std::make_tuple(filepath + "if.py", 4),               //
                             std::make_tuple(filepath + "loops.py", 7),            //
                             std::make_tuple(filepath + "many_lines.py", 11),      //
                             std::make_tuple(filepath + "many_parameters.py", 2),  //
                             std::make_tuple(filepath + "match_case.py", 8),       //
                             std::make_tuple(filepath + "nested_if.py", 9),        //
                             std::make_tuple(filepath + "simple.py", 6),           //
                             std::make_tuple(filepath + "ternary.py", 2)           //
                             ));

}  // namespace analyser::metric::metric_impl
