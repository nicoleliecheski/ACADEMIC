#include "ReportFilter.h"
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

string ReportFilter::formatCurrency(double value) const {
    long long cents = static_cast<long long>(llround(value * 100));
    long long integerPart = cents / 100;
    long long decimalPart = cents % 100;
    if (decimalPart < 0) {
        decimalPart = -decimalPart;
    }

    string integerStr = to_string(integerPart < 0 ? -integerPart : integerPart);
    string grouped;
    int count = 0;
    for (int i = static_cast<int>(integerStr.size()) - 1; i >= 0; --i) {
        grouped.insert(grouped.begin(), integerStr[i]);
        ++count;
        if (count % 3 == 0 && i != 0) {
            grouped.insert(grouped.begin(), '.');
        }
    }

    ostringstream oss;
    if (integerPart < 0) {
        oss << "-";
    }
    oss << grouped << "," << setw(2) << setfill('0') << decimalPart;
    return oss.str();
}

string ReportFilter::process(const AggregationResult& input) const {
    ostringstream oss;
    const string separator(40, '=');

    oss << separator << "\n";
    oss << "        RELATORIO DE VENDAS\n";
    oss << separator << "\n";
    oss << "\n";
    oss << "Vendas validas:    " << input.vendas_validas << "\n";
    oss << "Produtos vendidos: " << input.produtos_vendidos << "\n";
    oss << "Valor total:       R$ " << formatCurrency(input.valor_total) << "\n";
    oss << "\n";
    oss << separator << "\n";

    return oss.str();
}
