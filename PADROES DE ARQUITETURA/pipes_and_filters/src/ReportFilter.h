#ifndef REPORT_FILTER_H
#define REPORT_FILTER_H

#include <string>
#include "Record.h"

class ReportFilter {
public:
    std::string process(const AggregationResult& input) const;

private:
    std::string formatCurrency(double value) const;
};

#endif
