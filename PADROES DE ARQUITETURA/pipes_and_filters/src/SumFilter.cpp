#include "SumFilter.h"

using namespace std;

AggregationResult SumFilter::process(const vector<TransformedRecord>& input) const {
    AggregationResult result;

    for (const auto& record : input) {
        result.vendas_validas += 1;
        result.produtos_vendidos += record.quantidade;
        result.valor_total += record.valor_total;
    }

    return result;
}
