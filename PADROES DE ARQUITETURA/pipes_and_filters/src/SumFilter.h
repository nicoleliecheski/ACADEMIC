#ifndef SUM_FILTER_H
#define SUM_FILTER_H

#include <vector>
#include "Record.h"

using namespace std;

class SumFilter {
public:
    AggregationResult process(const vector<TransformedRecord>& input) const;
};

#endif
