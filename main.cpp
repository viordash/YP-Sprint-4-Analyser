#include "analyse.hpp"
#include "cmd_options.hpp"
#include "include/cmd_options.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"
#include <algorithm>
#include <cstdlib>
#include <print>
#include <string>
#include <unistd.h>

int main(int argc, char *argv[]) {
    analyser::cmd::ProgramOptions options;
    if (!options.Parse(argc, argv)) {
        return EXIT_FAILURE;
    }

    try {
        analyser::metric::MetricExtractor metric_extractor;
        metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CodeLinesCountMetric>());
        metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CyclomaticComplexityMetric>());
        metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CountParametersMetric>());

        auto analysis_results = analyser::AnalyseFunctions(options.GetFiles(), metric_extractor);

        std::ranges::for_each(analysis_results, [](const auto &analysis) {
            const auto &[func, metrics] = analysis;
            std::print("{}{}{}:", func.filename, (func.class_name ? "::" + *func.class_name : ""), "::" + func.name);

            std::println("");

            std::ranges::for_each(
                metrics, [](const auto &result) { std::println("\t{}: {}", result.metric_name, result.value); });
        });

        analyser::metric_accumulator::MetricsAccumulator accumulator;
        using namespace analyser::metric_accumulator::metric_accumulator_impl;
        accumulator.RegisterAccumulator("CodeLinesCount", std::make_unique<SumAverageAccumulator>());
        accumulator.RegisterAccumulator("CyclomaticComplexity", std::make_unique<AverageAccumulator>());
        accumulator.RegisterAccumulator("ParametersCount", std::make_unique<SumAverageAccumulator>());

        auto grouped_by_file = analyser::SplitByFiles(analysis_results);
        std::println("");
        std::ranges::for_each(grouped_by_file, [&](const auto &group) {
            const auto &[filename, results] = group;
            std::println("Accumulated Analysis for file {}:", filename);

            accumulator.ResetAccumulators();
            analyser::AccumulateFunctionAnalysis(results, accumulator);

            auto lines = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>("CodeLinesCount").Get();
            auto params = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>("ParametersCount").Get();
            auto complexity = accumulator.GetFinalizedAccumulator<AverageAccumulator>("CyclomaticComplexity").Get();

            std::println("\tCode Lines (Sum): {}", lines.sum);
            std::println("\tParameters (Sum): {}", params.sum);
            std::println("\tCyclomatic Complexity (Average): {:.2f}", complexity);
        });

        auto grouped_by_class = analyser::SplitByClasses(analysis_results);
        std::println("");
        std::ranges::for_each(grouped_by_class, [&](auto &group) {
            const auto &[classname, results] = group;
            std::println("Accumulated Analysis for class {}:", classname);

            accumulator.ResetAccumulators();
            analyser::AccumulateFunctionAnalysis(results, accumulator);

            auto lines = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>("CodeLinesCount").Get();
            auto params = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>("ParametersCount").Get();
            auto complexity = accumulator.GetFinalizedAccumulator<AverageAccumulator>("CyclomaticComplexity").Get();

            std::println("\tCode Lines (Sum): {}", lines.sum);
            std::println("\tParameters (Sum): {}", params.sum);
            std::println("\tCyclomatic Complexity (Average): {:.2f}", complexity);
        });

        std::println("\n--- General statistics ---");
        accumulator.ResetAccumulators();
        analyser::AccumulateFunctionAnalysis(analysis_results, accumulator);

        auto total_lines = accumulator.GetFinalizedAccumulator<SumAverageAccumulator>("CodeLinesCount").Get();
        auto avg_complexity = accumulator.GetFinalizedAccumulator<AverageAccumulator>("CyclomaticComplexity").Get();

        std::println("Total lines of code for all functions: {}", total_lines.sum);
        std::println("Average lines per function: {:.2f}", total_lines.average);
        std::println("Average cyclomatic complexity: {:.2f}", avg_complexity);

    } catch (const std::exception &e) {
        std::println(stderr, "Error: {}", e.what());
        return EXIT_FAILURE;
    }

    return 0;
}