#include "Pipeline.h"

std::string Pipeline::run(const std::string& filepath) const {
    auto rawRecords = readFilter.process(filepath);
    auto validRecords = cleanFilter.process(rawRecords);
    auto transformedRecords = transformFilter.process(validRecords);
    auto aggregation = sumFilter.process(transformedRecords);
    return reportFilter.process(aggregation);
}
