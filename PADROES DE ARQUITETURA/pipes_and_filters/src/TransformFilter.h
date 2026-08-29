#ifndef TRANSFORM_FILTER_H
#define TRANSFORM_FILTER_H

#include <vector>
#include "Record.h"

class TransformFilter {
public:
    std::vector<TransformedRecord> process(const std::vector<SaleRecord>& input) const;
};

#endif
