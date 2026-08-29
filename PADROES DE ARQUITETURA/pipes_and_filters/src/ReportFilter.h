#ifndef REPORT_FILTER_H
#define REPORT_FILTER_H

#include <string>
#include "Record.h"

using namespace std;

class ReportFilter {
public:
    string process(const AggregationResult& input) const;

private:
    string formatCurrency(double value) const;
};

#endif
