#include <cassert>
#include <iostream>
#include "../src/CleanFilter.h"
#include "../src/TransformFilter.h"
#include "../src/SumFilter.h"
#include "../src/ReportFilter.h"

using namespace std;

void testCleanFilterRemovesInvalidQuantity() {
    CleanFilter filter;
    vector<RawRecord> input = {
        {"001", "Mouse", "-1", "90.00"},
        {"002", "Teclado", "3", "150.00"}
    };
    auto output = filter.process(input);
    assert(output.size() == 1);
    assert(output[0].id_venda == "002");
}

void testCleanFilterRemovesMissingPrice() {
    CleanFilter filter;
    vector<RawRecord> input = {
        {"001", "Webcam", "2", ""},
        {"002", "Teclado", "3", "150.00"}
    };
    auto output = filter.process(input);
    assert(output.size() == 1);
    assert(output[0].id_venda == "002");
}

void testTransformFilterCalculatesTotal() {
    TransformFilter filter;
    vector<SaleRecord> input = {
        {"002", "Mouse", 5, 90.00}
    };
    auto output = filter.process(input);
    assert(output.size() == 1);
    assert(output[0].valor_total == 450.00);
}

void testSumFilterAggregates() {
    SumFilter filter;
    vector<TransformedRecord> input = {
        {"001", "Notebook", 2, 4500.00, 9000.00},
        {"002", "Mouse", 5, 90.00, 450.00}
    };
    auto result = filter.process(input);
    assert(result.vendas_validas == 2);
    assert(result.produtos_vendidos == 7);
    assert(result.valor_total == 9450.00);
}

void testReportFilterFormatsOutput() {
    ReportFilter filter;
    AggregationResult input;
    input.vendas_validas = 8;
    input.produtos_vendidos = 20;
    input.valor_total = 14630.00;
    string report = filter.process(input);
    assert(report.find("Vendas validas:    8") != string::npos);
    assert(report.find("Produtos vendidos: 20") != string::npos);
    assert(report.find("14.630,00") != string::npos);
}

int main() {
    testCleanFilterRemovesInvalidQuantity();
    testCleanFilterRemovesMissingPrice();
    testTransformFilterCalculatesTotal();
    testSumFilterAggregates();
    testReportFilterFormatsOutput();
    cout << "Todos os testes passaram." << endl;
    return 0;
}
