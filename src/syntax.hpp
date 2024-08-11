#define TOP Interpreter.Memory.top()

#define GETTOP TOP; Interpreter.Memory.pop()

#define CHECKSIZE(s) \
    if (Interpreter.Memory.size()<s) Interpreter.Error("Insufficient data in stack")

#define TRYOP(o) \
    CHECKSIZE(2); \
    auto a = GETTOP; auto b = GETTOP; \
    try { b o a; } \
    catch(...) { Interpreter.Error("Invalid operation"); }

#define CONVERT(d, t) \
    try{ \
        MemoryCell& a = TOP; \
        a.Value = a.Cast<t>(); \
        a.Type = d; \
    } \
    catch(...) { Interpreter.Error("Invalid cast:"); }

#define CHECKBLOCKS \
    if (Interpreter.Blocks.empty()) Interpreter.Error("Expected conditional block")


namespace SHOM {
    namespace Syntax {
        char Quote = '"';
        string Braces = "{}";
        string ArrBraces = "[]";

        unordered_map<char, char> Escape = {
            {'n', '\n'},
            {'t', '\t'},
            {'r', '\r'},
            {'\\', '\\'},
            {'"', '\"'},
        };

        unordered_map<char, function<void()>> Instructions = {
            {'(', [](){
                string s;
                cin >> s;
                Interpreter.Memory.push({s, String});
            }},
            {'~', [](){
                if (Interpreter.Memory.empty())
                    cout << "";

                else{
                    cout << TOP.Cast<string>();
                }
            }},
            {',', [](){
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
                CHECKSIZE(1);
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

            {'@', [](){
                CHECKSIZE(2);
                auto i = GETTOP;
                auto a = GETTOP;

                if (a.Type!=String && a.Type!=Array)
                    Interpreter.Error("Cannot apply indexing to primitive type: ", a.Cast<string>());

                if (i.Type!=Integer)
                    Interpreter.Error("Index must be an integer: ", i.Cast<string>());

                try {
                    auto ind = i.Cast<long long>();

                    if (a.Type==String)
                        Interpreter.Memory.push(MemoryCell({string(1, a.Cast<string>().at(ind)), String}));
                    else 
                        Interpreter.Memory.push(a.ArrayValue.at(ind));
                }
                catch(...){
                    Interpreter.Error("Invalid index: ", i.Cast<string>());
                }
            }},

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

                long long range = Interpreter.Memory.empty() ? 0 : TOP.Type==String ? TOP.Cast<string>().size() : TOP.Cast<long long>();

                string code = Interpreter.Blocks.back();
                Interpreter.Blocks.clear();

                Interpreter.Iterator = 0;
                for (long long i=0;i<range;i++){
                    Interpreter.InterpreteLine(code);
                    Interpreter.Iterator++;
                }
                Interpreter.Iterator = -1;
            }},
            {'i', [](){
                Interpreter.Iterator<0 ? 
                    Interpreter.Error("Iterator can only be accessed within a loop") : 
                    Interpreter.Memory.push({Interpreter.Iterator, Integer});
            }}
        };
    }
}