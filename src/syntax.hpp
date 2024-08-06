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
        unordered_map<char, char> Escape = {
            {'n', '\n'},
            {'t', '\t'},
            {'r', '\r'},
            {'\\', '\\'},
            {'"', '\"'},
        };

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
            {'=', [](){ TRYOP(==); }},
            {'!', [](){ TRYOP(!=); }},

            {'I', [](){ CONVERT(Integer, long long); }},
            {'D', [](){ CONVERT(Double, double); }},
            {'S', [](){ CONVERT(String, string); }},

            {'?', [](){
                while (Interpreter.Blocks.size()>2)
                    Interpreter.Blocks.pop_front();

                bool cond;
                if (Interpreter.Memory.empty())
                    cond = 0;
                else{
                    Interpreter.Memory.top() & MemoryCell({1, Integer});
                    cond = Interpreter.Memory.top().Cast<long long>();
                    Interpreter.Memory.pop();
                }

                string code = Interpreter.Blocks[!cond];

                while (!Interpreter.Blocks.empty())
                    Interpreter.Blocks.pop_back();
                
                Interpreter.InterpreteLine(code);
            }},
        };
    }
}