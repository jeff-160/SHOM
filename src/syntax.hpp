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
    CHECKSIZE(1); \
    try{ \
        auto& a = TOP; \
        a.Value = a.Cast<t>(); \
        a.Type = d; \
    } \
    catch(...) { Interpreter.Error("Invalid cast:"); }

#define CHECKBLOCKS \
    if (Interpreter.Blocks.empty()) Interpreter.Error("Expected conditional block")

#define CHECKINDEX \
    if (i.Type!=Integer) Interpreter.Error("Index must be an integer: ", i.Cast<string>());

#define OOBINDEX \
    Interpreter.Error("Invalid index: ", i.Cast<string>());

#define CHECKARR(s) \
    if (a.Type!=Array) Interpreter.Error(s, a.Cast<string>());


namespace SHOM {
    namespace Syntax {
        char Quote = '"';
        char Comment = '#';
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
            {'$', [](){
                string s;
                getline(cin, s);
                Interpreter.Memory.push({s, String});
            }},
            {'~', [](){
                cout << (Interpreter.Memory.empty() ? "" : TOP.Cast<string>());
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
                CHECKSIZE(2);
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
                auto& a = TOP;

                if (a.Type!=String && a.Type!=Array)
                    Interpreter.Error("Cannot apply indexing to primitive type: ", a.Cast<string>());

                CHECKINDEX;

                try {
                    auto ind = i.Cast<long long>();

                    if (a.Type==String)
                        Interpreter.Memory.push(MemoryCell({string(1, a.Cast<string>().at(ind)), String}));
                    else 
                        Interpreter.Memory.push(a.ArrayValue.at(ind));
                }
                catch(...){ OOBINDEX; }
            }},
            {'(', [](){
                CHECKSIZE(2);
                auto e = GETTOP;
                auto& a = TOP;

                CHECKARR("Cannot add element to non-array: ");
                
                a.ArrayValue.push_back(e);
            }},
            {')', [](){
                CHECKSIZE(2);
                auto i = GETTOP;
                auto& a = TOP;

                CHECKARR("Cannot remove element from non-array: ");
                CHECKINDEX;

                auto ind = i.Cast<long long>();
                if (ind<0 || ind>=a.Size())
                    OOBINDEX;

                a.ArrayValue.erase(a.ArrayValue.begin()+ind);
            }},
            {'`', [](){
                CHECKSIZE(2);
                auto e = TOP.Cast<string>(); Interpreter.Memory.pop();
                auto& a = TOP;

                CHECKARR("Cannot get index of element from non-array");

                long long ind = -1;
                for (size_t i=0;i<a.ArrayValue.size();i++){
                    if (a.ArrayValue[i].Cast<string>()==e){
                        ind = i;
                        break;
                    }
                }

                Interpreter.Memory.push({ind, Integer});
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

                auto& a = TOP;
                long long range = Interpreter.Memory.empty() ? 0 : a.Type==String || a.Type==Array ? a.Size() : a.Cast<long long>();

                string code = Interpreter.Blocks.back();
                Interpreter.Blocks.clear();

                Interpreter.Iterators.push_back(0);
                long long i = 0;
                while (i!=range){
                    Interpreter.InterpreteLine(code);
                    Interpreter.Iterators.back()++;
                    i++;
                }
                Interpreter.Iterators.pop_back();
            }},
            {'i', [](){
                Interpreter.Iterators.empty() ? 
                    Interpreter.Error("Iterator can only be accessed within a loop") : 
                    Interpreter.Memory.push({Interpreter.Iterators.back(), Integer});
            }},
        };
    }
}