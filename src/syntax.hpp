#define TOP Interpreter.Memory.top(); Interpreter.Memory.pop()
#define GETAB auto a = TOP; auto b = TOP;

namespace SHOM {
    namespace Syntax {
        unordered_map<char, function<void()>> Instructions = {
            {'~', [](){
                cout << (Interpreter.Memory.empty() ? "" : Interpreter.Memory.top().Value);
            }},

            {'`', [](){
                if (!Interpreter.Memory.empty())
                    Interpreter.Memory.pop();
            }},

            {'_', [](){
                while (!Interpreter.Memory.empty())
                    Interpreter.Memory.pop();
            }},

            {'+', [](){
                GETAB;
                b+a;
            }},

            {'-', [](){
                    GETAB;
                    b-a;
            }},

            {'*', [](){
                    GETAB;
                    b*a;
            }},

            {'/', [](){
                    GETAB;
                    b/a;
            }},
        };
    }
}