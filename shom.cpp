#include "src/include.hpp"
#define TIME high_resolution_clock::now()
using namespace SHOM;

int main(int argc, char* argv[]){
    if (argc<2)
        Interpreter.Error("No input file", "", 0);

    auto rev = [](string s) -> string {
        reverse(s.begin(), s.end());
        return s;
    };

    ifstream file(argv[1]);
    if (!file.is_open())
        Interpreter.Error("Invalid file path", "", 0);
    if (rev(argv[1]).find(rev(".shom"))!=0)
        Interpreter.Error("Input file lacks .shom extension", "", 0);

    auto start = TIME;
    Interpreter.File = argv[1];
    Interpreter.Interprete(file);

    cout << "\nProcess exited after " << (float)(duration_cast<microseconds>(TIME-start).count())/1e6 << " seconds";
    file.close();
}