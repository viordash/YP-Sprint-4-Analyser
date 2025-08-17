#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

TEST(CyclomaticComplexityMetricTests, get_metric_name) {
    CyclomaticComplexityMetric metric;
    auto func = function::Function{.ast = "(function_definition [10, 0] - [14, 15] body: (block ...))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.metric_name, "CyclomaticComplexity");
}

TEST(CyclomaticComplexityMetricTests, simple_function) {
    CyclomaticComplexityMetric metric;
    auto func = function::Function{.ast = R"((function_definition body: (block (expression_statement (assignment)))))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 1);
}

TEST(CyclomaticComplexityMetricTests, single_if_statement) {
    CyclomaticComplexityMetric metric;
    auto func = function::Function{.ast = R"((function_definition body: (block (if_statement))))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 2);
}

TEST(CyclomaticComplexityMetricTests, if_elif_else_statement) {
    CyclomaticComplexityMetric metric;
    auto func = function::Function{.ast = R"((function_definition body: (block 
        (if_statement [1, 4] - [7, 20]
        condition: (comparison_operator [1, 7] - [1, 12]
          (if_statement [2, 8] - [7, 20]
            condition: (comparison_operator [2, 11] - [2, 16]
            alternative: (elif_clause [4, 8] - [5, 20]
              condition: (comparison_operator [4, 13] - [4, 18]
              consequence: (block [5, 12] - [5, 20]
                (return_statement [5, 12] - [5, 20]
            alternative: (else_clause [6, 8] - [7, 20])"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 4);
}

TEST(CyclomaticComplexityMetricTests, nested_loops_and_ifs) {
    CyclomaticComplexityMetric metric;
    auto func = function::Function{.ast = R"((function_definition body: (block 
            (for_statement 
                (while_statement 
                    (if_statement))))))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 4);
}

TEST(CyclomaticComplexityMetricTests, try_except_block) {
    CyclomaticComplexityMetric metric;
    auto func = function::Function{.ast = R"((function_definition body: (block 
            (try_statement 
                (except_clause) 
                    (except_clause)))))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 3);
}

TEST(CyclomaticComplexityMetricTests, match_case_statement) {
    CyclomaticComplexityMetric metric;
    auto func = function::Function{.ast = R"((function_definition body: (block 
            (match_statement 
                (case_pattern) 
                (case_pattern) 
                (case_pattern)))))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 4);
}

TEST(CyclomaticComplexityMetricTests, ternary_and_assert) {
    CyclomaticComplexityMetric metric;
    auto func = function::Function{.ast = R"((function_definition body: (block 
                (assert_statement) 
                (expression_statement 
                (conditional_expression)))))"};

    auto result = metric.Calculate(func);
    ASSERT_EQ(result.value, 3);
}

class CyclomaticComplexityFromTestFiles : public ::testing::TestWithParam<std::tuple<std::string, int>> {};

TEST_P(CyclomaticComplexityFromTestFiles, from_test_file) {
    auto filename = std::get<0>(GetParam());
    auto count = std::get<1>(GetParam());

    CyclomaticComplexityMetric metric;
    function::FunctionExtractor func_extractor;
    auto file = file::File(std::get<0>(GetParam()));

    auto func = func_extractor.Get(file);
    ASSERT_EQ(func.size(), 1);
    auto result = metric.Calculate(func[0]);
    ASSERT_EQ(result.value, std::get<1>(GetParam()));
}

const static std::string filepath = "../src/metric_impl/tests/files/";
INSTANTIATE_TEST_SUITE_P(_, CyclomaticComplexityFromTestFiles,
                         ::testing::Values(                                        //
                             std::make_tuple(filepath + "comments.py", 1),         //
                             std::make_tuple(filepath + "exceptions.py", 4),       //
                             std::make_tuple(filepath + "if.py", 2),               //
                             std::make_tuple(filepath + "loops.py", 4),            //
                             std::make_tuple(filepath + "many_lines.py", 2),       //
                             std::make_tuple(filepath + "many_parameters.py", 2),  //
                             std::make_tuple(filepath + "match_case.py", 4),       //
                             std::make_tuple(filepath + "nested_if.py", 5),        //
                             std::make_tuple(filepath + "simple.py", 2),           //
                             std::make_tuple(filepath + "ternary.py", 3)           //
                             ));

}  // namespace analyser::metric::metric_impl
