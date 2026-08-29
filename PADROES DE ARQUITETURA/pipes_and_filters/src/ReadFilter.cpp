#include "ReadFilter.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<RawRecord> ReadFilter::process(const std::string& filepath) const {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + filepath);
    }

    std::vector<RawRecord> records;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> fields;

        while (std::getline(ss, field, ',')) {
            fields.push_back(field);
        }
        while (fields.size() < 4) {
            fields.push_back("");
        }

        RawRecord record;
        record.id_venda = fields[0];
        record.produto = fields[1];
        record.quantidade_str = fields[2];
        record.preco_unitario_str = fields[3];

        records.push_back(record);
    }

    return records;
}
