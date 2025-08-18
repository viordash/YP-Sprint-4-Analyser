

#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(CategoricalAccumulatorTests, BasicFunctionality) {
    CategoricalAccumulator acc;
    acc.Accumulate({.value = 5});
    acc.Accumulate({.value = 10});
    acc.Accumulate({.value = 5});
    acc.Accumulate({.value = 5});
    acc.Accumulate({.value = 20});
    acc.Finalize();

    const std::unordered_map<int, int> expected = {{5, 3}, {10, 1}, {20, 1}};
    ASSERT_EQ(acc.Get(), expected);
}

TEST(CategoricalAccumulatorTests, SingleValue) {
    CategoricalAccumulator acc;
    acc.Accumulate({.value = 100});
    acc.Finalize();

    const std::unordered_map<int, int> expected = {{100, 1}};
    ASSERT_EQ(acc.Get(), expected);
}

TEST(CategoricalAccumulatorTests, Empty) {
    CategoricalAccumulator acc;
    acc.Finalize();

    ASSERT_TRUE(acc.Get().empty());
}

TEST(CategoricalAccumulatorTests, Reset) {
    CategoricalAccumulator acc;
    acc.Accumulate({.value = 1});
    acc.Accumulate({.value = 1});
    acc.Finalize();
    ASSERT_EQ(acc.Get(), (std::unordered_map<int, int>{{1, 2}}));

    acc.Reset();
    acc.Accumulate({.value = 99});
    acc.Finalize();

    ASSERT_EQ(acc.Get(), (std::unordered_map<int, int>{{99, 1}}));
}

TEST(CategoricalAccumulatorTests, ThrowsOnGetBeforeFinalize) {
    CategoricalAccumulator acc;
    acc.Accumulate({.value = 10});

    EXPECT_THROW(acc.Get(), std::logic_error);
}

TEST(CategoricalAccumulatorTests, ThrowsOnAccumulateAfterFinalize) {
    CategoricalAccumulator acc;
    acc.Accumulate({.value = 10});
    acc.Finalize();

    EXPECT_THROW(acc.Accumulate({.value = 20}), std::logic_error);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
