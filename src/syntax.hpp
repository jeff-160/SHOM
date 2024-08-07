#define TOP Interpreter.Memory.top()

#define GETTOP TOP; Interpreter.Memory.pop()

#define TRYOP(o) \
    if (Interpreter.Memory.size()<2) \
        Interpreter.Error("Insufficient data in stack"); \
    auto a = GETTOP; auto b = GETTOP; \
    try { b o a; } \
    catch(...) { Interpreter.Error("Mismatched types"); }

#define CONVERT(d, t) \
    try{ \
        MemoryCell& a = TOP; \
        a.Value = a.Cast<t>(); \
        a.Type = d; \
    } \
    catch(...) { Interpreter.Error("Invalid cast"); }

#define CHECKBLOCKS \
    if (Interpreter.Blocks.empty()) Interpreter.Error("Expected conditional block")


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
                    visit([](auto&& arg){ cout << arg; }, TOP.Value);
            }},
            {'`', [](){
                if (!Interpreter.Memory.empty())
                    Interpreter.Memory.pop();
            }},
            {'_', [](){
                while (!Interpreter.Memory.empty())
                    Interpreter.Memory.pop();
            }},
            {'\\', [](){
                auto a = GETTOP;
                auto b = GETTOP;

                Interpreter.Memory.push(a);
                Interpreter.Memory.push(b);
            }},
            {';', [](){
                Interpreter.Memory.push(TOP);
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
                CHECKBLOCKS;

                bool cond;
                if (Interpreter.Memory.empty())
                    cond = 0;
                else{
                    TOP & MemoryCell({1, Integer});
                    cond = TOP.Cast<long long>();
                    Interpreter.Memory.pop();
                }

                string code;
                try{
                    size_t s = Interpreter.Blocks.size();
                    code = Interpreter.Blocks.at(s>=2 ? s-1-cond : !cond);
                }
                catch(...){
                    code = "";
                }

                Interpreter.Blocks.clear();
                Interpreter.InterpreteLine(code);
            }},
            {':', [](){
                CHECKBLOCKS;

                long long range = TOP.Type==String ? TOP.Cast<string>().size() : TOP.Cast<long long>();

                string code = Interpreter.Blocks.back();
                Interpreter.Blocks.clear();

                for (long long i=0;i<range;i++)
                    Interpreter.InterpreteLine(code);
            }}
        };
    }
}