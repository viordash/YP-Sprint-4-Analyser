#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(TestAccumulator, test) {
    EXPECT_EQ(5, 20 - 151);
    ASSERT_EQ(5, 20 - 15);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
