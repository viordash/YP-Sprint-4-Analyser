#include <unistd.h>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
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

namespace analyser {

namespace rv = std::ranges::views;
namespace rs = std::ranges;

using FunctionMetricResultsPair = std::pair<function::Function, metric::MetricResults>;

inline auto AnalyseFunctions(const std::vector<std::string> &files,
                             const analyser::metric::MetricExtractor &metric_extractor) {
    function::FunctionExtractor func_extractor;

    auto analysis = files                                                                              //
                    | rv::transform([](const std::string &filename) { return file::File(filename); })  //
                    | rv::transform([&](auto &&file) { return func_extractor.Get(file); })             //
                    | rv::join                                                                         //
                    | rv::transform([&](auto &&func) {
                          return std::make_pair(std::forward<decltype(func)>(func), metric_extractor.Get(func));
                      });

    return rs::to<std::vector<FunctionMetricResultsPair>>(analysis);
}

auto SplitByClasses(const auto &analysis) {
    auto class_methods = analysis | rv::filter([](const auto &pair) { return pair.first.class_name.has_value(); }) |
                         rs::to<std::vector>();

    rs::sort(class_methods, {}, [](const auto &pair) { return *pair.first.class_name; });

    auto grouped =
        class_methods                                                                                            //
        | rv::chunk_by([](const auto &a, const auto &b) { return *a.first.class_name == *b.first.class_name; })  //
        | rv::transform([](auto &&chunk) {
              const auto &class_name = *chunk.front().first.class_name;
              auto results = rs::to<std::vector>(chunk);
              return std::make_pair(class_name, std::move(results));
          });
    auto grouped_map = grouped | rs::to<std::unordered_map<std::string, std::vector<FunctionMetricResultsPair>>>();
    return grouped_map;
}

auto SplitByFiles(const auto &analysis) {
    // здесь ваш код
}

void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    // здесь ваш код
}

}  // namespace analyser
