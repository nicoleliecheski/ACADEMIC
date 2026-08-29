#include "Pipeline.h"

using namespace std;

string Pipeline::run(const string& filepath) const {
    auto rawRecords = readFilter.process(filepath);
    auto validRecords = cleanFilter.process(rawRecords);
    auto transformedRecords = transformFilter.process(validRecords);
    auto aggregation = sumFilter.process(transformedRecords);
    return reportFilter.process(aggregation);
}
