#include "CleanFilter.h"
#include <cstdlib>
#include <cctype>

namespace {

bool parseInt(const std::string& str, int& out) {
    if (str.empty()) {
        return false;
    }
    size_t i = 0;
    if (str[0] == '-' || str[0] == '+') {
        i = 1;
    }
    if (i >= str.size()) {
        return false;
    }
    for (; i < str.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(str[i]))) {
            return false;
        }
    }
    out = std::atoi(str.c_str());
    return true;
}

bool parseDouble(const std::string& str, double& out) {
    if (str.empty()) {
        return false;
    }
    char* end = nullptr;
    double value = std::strtod(str.c_str(), &end);
    if (end == str.c_str() || *end != '\0') {
        return false;
    }
    out = value;
    return true;
}

}

bool CleanFilter::isValid(const RawRecord& record, int& quantidade, double& preco) const {
    if (!parseInt(record.quantidade_str, quantidade)) {
        return false;
    }
    if (quantidade <= 0) {
        return false;
    }
    if (!parseDouble(record.preco_unitario_str, preco)) {
        return false;
    }
    if (preco < 0) {
        return false;
    }
    return true;
}

std::vector<SaleRecord> CleanFilter::process(const std::vector<RawRecord>& input) const {
    std::vector<SaleRecord> output;

    for (const auto& raw : input) {
        int quantidade = 0;
        double preco = 0.0;

        if (!isValid(raw, quantidade, preco)) {
            continue;
        }

        SaleRecord sale;
        sale.id_venda = raw.id_venda;
        sale.produto = raw.produto;
        sale.quantidade = quantidade;
        sale.preco_unitario = preco;

        output.push_back(sale);
    }

    return output;
}
