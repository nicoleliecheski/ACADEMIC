#ifndef PIPELINE_H
#define PIPELINE_H

#include <string>
#include "ReadFilter.h"
#include "CleanFilter.h"
#include "TransformFilter.h"
#include "SumFilter.h"
#include "ReportFilter.h"

using namespace std;

class Pipeline {
public:
    string run(const string& filepath) const;

private:
    ReadFilter readFilter;
    CleanFilter cleanFilter;
    TransformFilter transformFilter;
    SumFilter sumFilter;
    ReportFilter reportFilter;
};

#endif
