#include <unistd.h>

#include "analyse.hpp"
#include "cmd_options.hpp"
#include "file.hpp"
#include "function.hpp"
#include "include/cmd_options.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

int main(int argc, char *argv[]) {
    analyser::cmd::ProgramOptions options;
    if (!options.Parse(argc, argv)) {
        return EXIT_FAILURE;
    }

    if (options.GetFiles().size() != 1) {
        return EXIT_FAILURE;
    }

    try {
        analyser::metric::MetricExtractor metric_extractor;
        metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CodeLinesCountMetric>());
        metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CyclomaticComplexityMetric>());
        metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CountParametersMetric>());

        auto analysis_results = analyser::AnalyseFunctions(options.GetFiles(), metric_extractor);

        std::println("Analysis complete. Found {} functions.\n", analysis_results.size());
        std::ranges::for_each(analysis_results, [](const auto &analysis) {
            const auto &[func, metrics] = analysis;
            std::println("Function: {} in file: {}", func.name, func.filename);
            std::ranges::for_each(
                metrics, [](const auto &result) { std::println("  - {}: {}", result.metric_name, result.value); });
            std::println("");
        });

        auto grouped_by_class = analyser::SplitByClasses(analysis_results);
        std::println("\n--- Grouped by Class ---\n");
        std::ranges::for_each(grouped_by_class, [](const auto &group) {
            const auto &[class_name, results] = group;
            std::println("Class: {}", class_name);

            std::ranges::for_each(results, [](const auto &analysis) {
                const auto &[func, metrics] = analysis;
                std::println("  Function: {}", func.name);

                std::ranges::for_each(metrics, [](const auto &result) {
                    std::println("    - {}: {}", result.metric_name, result.value);
                });
            });
            std::println("");
        });

        auto grouped_by_file = analyser::SplitByFiles(analysis_results);
        std::println("\n--- Grouped by File ---\n");
        std::ranges::for_each(grouped_by_file, [](const auto &group) {
            const auto &[filename, results] = group;
            std::println("File: {}", filename);

            std::ranges::for_each(results, [](const auto &analysis) {
                const auto &[func, metrics] = analysis;
                std::println("  Function: {}.{}", func.class_name.has_value() ? *func.class_name : "<global>",
                             func.name);

                std::ranges::for_each(metrics, [](const auto &result) {
                    std::println("    - {}: {}", result.metric_name, result.value);
                });
            });
            std::println("");
        });

    } catch (const std::exception &e) {
        std::println(stderr, "Error: {}", e.what());
        return EXIT_FAILURE;
    }

    // analyser::metric_accumulator::MetricsAccumulator accumulator;
    // зарегистрируйте аккумуляторы метрик в accumulator

    // запустите analyser::SplitByFiles
    // запустите analyser::AccumulateFunctionAnalysis для каждого подмножества результатов метрик
    // выведете результаты на консоль

    // запустите analyser::SplitByClasses
    // запустите analyser::AccumulateFunctionAnalysis для каждого подмножества результатов метрик
    // выведете результаты на консоль

    // запустите analyser::AccumulateFunctionAnalysis для всех результатов метрик
    // выведете результаты на консоль

    return 0;
}
