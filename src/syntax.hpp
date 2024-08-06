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
                    TOP & MemoryCell({1, Integer});
                    cond = TOP.Cast<long long>();
                    Interpreter.Memory.pop();
                }

                string code;
                
                try{
                    code = Interpreter.Blocks.at(!cond);
                }
                catch(...){
                    code = "";
                }

                Interpreter.Blocks.clear();
                
                Interpreter.InterpreteLine(code);
            }},

            {':', [](){
                long long range = TOP.Type==String ? TOP.Cast<string>().size() : TOP.Cast<long long>();
            }}
        };
    }
}