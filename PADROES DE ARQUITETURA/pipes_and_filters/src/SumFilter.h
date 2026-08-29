#ifndef SUM_FILTER_H
#define SUM_FILTER_H

#include <vector>
#include "Record.h"

class SumFilter {
public:
    AggregationResult process(const std::vector<TransformedRecord>& input) const;
};

#endif
