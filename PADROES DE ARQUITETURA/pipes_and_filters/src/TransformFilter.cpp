#include "TransformFilter.h"

std::vector<TransformedRecord> TransformFilter::process(const std::vector<SaleRecord>& input) const {
    std::vector<TransformedRecord> output;

    for (const auto& sale : input) {
        TransformedRecord transformed;
        transformed.id_venda = sale.id_venda;
        transformed.produto = sale.produto;
        transformed.quantidade = sale.quantidade;
        transformed.preco_unitario = sale.preco_unitario;
        transformed.valor_total = sale.quantidade * sale.preco_unitario;

        output.push_back(transformed);
    }

    return output;
}
