#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(AverageAccumulatorTests, SingleValue) {
    AverageAccumulator acc;
    const metric::MetricResult r1 = {.value = 42};

    acc.Accumulate(r1);
    acc.Finalize();
    ASSERT_DOUBLE_EQ(acc.Get(), 42.0);
}

TEST(AverageAccumulatorTests, MultipleValue) {
    AverageAccumulator acc;
    const metric::MetricResult r1 = {.value = 10};
    const metric::MetricResult r2 = {.value = 25};
    const metric::MetricResult r3 = {.value = 35};

    acc.Accumulate(r1);
    acc.Accumulate(r2);
    acc.Accumulate(r3);
    acc.Finalize();
    ASSERT_NEAR(acc.Get(), 23.33333333, 0.00001);
}

TEST(AverageAccumulatorTests, Empty) {
    AverageAccumulator acc;

    acc.Finalize();
    ASSERT_DOUBLE_EQ(acc.Get(), 0.0);
}

TEST(AverageAccumulatorTests, Reset) {
    AverageAccumulator acc;
    acc.Accumulate({.value = 100});
    acc.Accumulate({.value = 200});
    acc.Finalize();
    ASSERT_DOUBLE_EQ(acc.Get(), 150.0);

    acc.Reset();
    acc.Accumulate({.value = 5});
    acc.Accumulate({.value = 15});
    acc.Finalize();
    ASSERT_DOUBLE_EQ(acc.Get(), 10.0);
}

TEST(AverageAccumulatorTests, ThrowsOnGetBeforeFinalize) {
    AverageAccumulator acc;
    acc.Accumulate({.value = 10});

    EXPECT_THROW(acc.Get(), std::logic_error);
}

TEST(AverageAccumulatorTests, ThrowsOnAccumulateAfterFinalize) {
    AverageAccumulator acc;
    acc.Accumulate({.value = 10});
    acc.Finalize();

    EXPECT_THROW(acc.Accumulate({.value = 20}), std::logic_error);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
