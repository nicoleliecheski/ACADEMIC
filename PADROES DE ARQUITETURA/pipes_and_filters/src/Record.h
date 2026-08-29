#ifndef RECORD_H
#define RECORD_H

#include <string>

struct RawRecord {
    std::string id_venda;
    std::string produto;
    std::string quantidade_str;
    std::string preco_unitario_str;
};

struct SaleRecord {
    std::string id_venda;
    std::string produto;
    int quantidade;
    double preco_unitario;
};

struct TransformedRecord {
    std::string id_venda;
    std::string produto;
    int quantidade;
    double preco_unitario;
    double valor_total;
};

struct AggregationResult {
    int vendas_validas = 0;
    int produtos_vendidos = 0;
    double valor_total = 0.0;
};

#endif
