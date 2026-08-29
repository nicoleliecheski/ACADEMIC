#ifndef TRANSFORM_FILTER_H
#define TRANSFORM_FILTER_H

#include <vector>
#include "Record.h"

using namespace std;

class TransformFilter {
public:
    vector<TransformedRecord> process(const vector<SaleRecord>& input) const;
};

#endif
