#ifndef CLEAN_FILTER_H
#define CLEAN_FILTER_H

#include <vector>
#include "Record.h"

using namespace std;

class CleanFilter {
public:
    vector<SaleRecord> process(const vector<RawRecord>& input) const;

private:
    bool isValid(const RawRecord& record, int& quantidade, double& preco) const;
};

#endif
