#define TOP Interpreter.Memory.top(); Interpreter.Memory.pop()
#define TRYOP(o) \
    if (Interpreter.Memory.size()<2) \
        Interpreter.Error("Insufficient data in stack"); \
    auto a = TOP; auto b = TOP; \
    try { b o a; } \
    catch(...) { Interpreter.Error("Mismatched types"); }
#define CONVERT(d, t) \
    MemoryCell& a = Interpreter.Memory.top(); \
    a.Value = a.Cast<t>(); \
    a.Type = d; \


namespace SHOM {
    namespace Syntax {
        unordered_map<char, function<void()>> Instructions = {
            {'~', [](){
                if (Interpreter.Memory.empty())
                    cout << "";
                else
                    visit([](auto&& arg){ cout << arg; }, Interpreter.Memory.top().Value);
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
            {'^', [](){ TRYOP(^); }},
            {'%', [](){ TRYOP(%); }},

            {'>', [](){ TRYOP(>); }},
            {'<', [](){ TRYOP(<); }},
            
            {'&', [](){ TRYOP(&); }},
            {'|', [](){ TRYOP(|); }},
            {'=', [](){ TRYOP(=); }},

            {'I', [](){ CONVERT(Integer, long long); }},
            {'D', [](){ CONVERT(Double, double); }},
            {'S', [](){ CONVERT(String, string); }},
        };
    }
}