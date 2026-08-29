#ifndef READ_FILTER_H
#define READ_FILTER_H

#include <string>
#include <vector>
#include "Record.h"

class ReadFilter {
public:
    std::vector<RawRecord> process(const std::string& filepath) const;
};

#endif
