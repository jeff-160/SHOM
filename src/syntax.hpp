#define TOP Interpreter.Memory.top(); Interpreter.Memory.pop()
#define TRYOP(o) \
    auto a = TOP; auto b = TOP; \
    try { b o a; } \
    catch(...) { Interpreter.Error("Mismatched types"); }


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

            {'+', [](){ TRYOP(+); }},
            {'-', [](){ TRYOP(-); }},
            {'*', [](){ TRYOP(*); }},
            {'/', [](){ TRYOP(/); }},

            {'&', [](){ TRYOP(&); }},
            {'|', [](){ TRYOP(|); }},
            {'=', [](){ TRYOP(=); }},
        };
    }
}