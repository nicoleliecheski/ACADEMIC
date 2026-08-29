#include <iostream>
#include "Pipeline.h"

using namespace std;

int main(int argc, char* argv[]) {

    Pipeline pipeline;

    try {
        string report = pipeline.run(argv[1]);
        cout << report << endl;
    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }

    return 0;
}
