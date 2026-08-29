#ifndef CLEAN_FILTER_H
#define CLEAN_FILTER_H

#include <vector>
#include "Record.h"

class CleanFilter {
public:
    std::vector<SaleRecord> process(const std::vector<RawRecord>& input) const;

private:
    bool isValid(const RawRecord& record, int& quantidade, double& preco) const;
};

#endif
