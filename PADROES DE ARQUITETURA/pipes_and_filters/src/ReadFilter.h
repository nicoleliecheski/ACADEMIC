#ifndef READ_FILTER_H
#define READ_FILTER_H

#include <string>
#include <vector>
#include "Record.h"

using namespace std;

class ReadFilter {
public:
    vector<RawRecord> process(const string& filepath) const;
};

#endif
