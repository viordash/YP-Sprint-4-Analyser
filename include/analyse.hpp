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

    return rs::to<std::vector<std::pair<function::Function, metric::MetricResults>>>(analysis);
}

auto SplitByClasses(const auto &analysis) {
    // здесь ваш код
}

auto SplitByFiles(const auto &analysis) {
    // здесь ваш код
}

void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    // здесь ваш код
}

}  // namespace analyser
