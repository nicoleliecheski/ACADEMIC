#include "ReadFilter.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

vector<RawRecord> ReadFilter::process(const string& filepath) const {
    ifstream file(filepath);
    if (!file.is_open()) {
        throw runtime_error("Nao foi possivel abrir o arquivo: " + filepath);
    }

    vector<RawRecord> records;
    string line;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string field;
        vector<string> fields;

        while (getline(ss, field, ',')) {
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
