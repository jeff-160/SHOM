#include "src/include.hpp"

int main(int argc, char* argv[]){
    if (argc<2)
        cout << "Error: No input file";

    ifstream file(argv[1]);
    SHOM::Interpreter.File = argv[1];

    auto start = high_resolution_clock::now();
    SHOM::Interpreter.Interprete(file);

    cout << "\nProcess exited after " << (float)(duration_cast<microseconds>(high_resolution_clock::now()-start).count())/1e6 << " seconds";
    file.close();
}