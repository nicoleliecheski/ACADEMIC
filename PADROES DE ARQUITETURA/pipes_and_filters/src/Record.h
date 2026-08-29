#ifndef RECORD_H
#define RECORD_H

using namespace std;

#include <string>

struct RawRecord {
    string id_venda;
    string produto;
    string quantidade_str;
    string preco_unitario_str;
};

struct SaleRecord {
    string id_venda;
    string produto;
    int quantidade;
    double preco_unitario;
};

struct TransformedRecord {
    string id_venda;
    string produto;
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
