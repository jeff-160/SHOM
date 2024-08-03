#include "src/include.hpp"

int main(int argc, char* argv[]){
    if (argc<2)
        cout << "Error: No input file";

    ifstream file(argv[1]);
    SHOM::Interpreter.File = argv[1];
    SHOM::Interpreter.Interprete(file);
    file.close();
}