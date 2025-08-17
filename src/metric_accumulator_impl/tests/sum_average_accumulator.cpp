#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(SumAverageAccumulatorTests, SingleValue) {
    SumAverageAccumulator acc;
    const metric::MetricResult r1 = {.value = 42};

    acc.Accumulate(r1);
    acc.Finalize();

    ASSERT_EQ(acc.Get(), (SumAverageAccumulator::SumAverage{42, 42.0}));
}

TEST(SumAverageAccumulatorTests, MultipleValue) {
    SumAverageAccumulator acc;
    const metric::MetricResult r1 = {.value = 10};
    const metric::MetricResult r2 = {.value = 25};
    const metric::MetricResult r3 = {.value = 35};

    acc.Accumulate(r1);
    acc.Accumulate(r2);
    acc.Accumulate(r3);
    acc.Finalize();

    auto result = acc.Get();
    ASSERT_EQ(result.sum, 70);
    ASSERT_NEAR(result.average, 23.33333333, 0.00001);
}

TEST(SumAverageAccumulatorTests, Empty) {
    SumAverageAccumulator acc;
    acc.Finalize();
    ASSERT_EQ(acc.Get(), (SumAverageAccumulator::SumAverage{0, 0.0}));
}

TEST(SumAverageAccumulatorTests, Reset) {
    SumAverageAccumulator acc;
    acc.Accumulate({.value = 100});
    acc.Accumulate({.value = 200});
    acc.Finalize();
    ASSERT_EQ(acc.Get(), (SumAverageAccumulator::SumAverage{300, 150.0}));

    acc.Reset();
    acc.Accumulate({.value = 5});
    acc.Accumulate({.value = 15});
    acc.Finalize();
    ASSERT_EQ(acc.Get(), (SumAverageAccumulator::SumAverage{20, 10.0}));
}

TEST(SumAverageAccumulatorTests, ThrowsOnGetBeforeFinalize) {
    SumAverageAccumulator acc;
    acc.Accumulate({.value = 10});

    EXPECT_THROW(acc.Get(), std::logic_error);
}

TEST(SumAverageAccumulatorTests, ThrowsOnAccumulateAfterFinalize) {
    SumAverageAccumulator acc;
    acc.Accumulate({.value = 10});
    acc.Finalize();

    EXPECT_THROW(acc.Accumulate({.value = 20}), std::logic_error);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
