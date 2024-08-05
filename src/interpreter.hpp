#define NUMOP(a, b, o, f, t) \
    ostringstream oss; \
    oss << setprecision(8) << noshowpoint << f(a) o f(b); \
    Interpreter.Memory.push({oss.str(), t})

#define NUMOPS(o) \
    else if (this->Type==Integer) { \
        NUMOP(this->Value, a.Value, o, stoll, Integer); \
    } \
    else if (this->Type==Double) { \
        NUMOP(this->Value, a.Value, o, stod, Double); \
    }

#define STROP(a, b, o) \
    Interpreter.Memory.push({a o b, String})

namespace SHOM {
    enum DataType {
        Null,
        Integer,
        Double,
        String
    };

    struct MemoryCell {
        string Value;
        DataType Type;

        void operator+(MemoryCell const& a);
        void operator-(MemoryCell const& a);
        void operator*(MemoryCell const& a);
        void operator/(MemoryCell const& a);
        void operator|(MemoryCell const& a);
        void operator&(MemoryCell const& a);
        void operator=(MemoryCell const& a);
        void operator>(MemoryCell const& a);
        void operator<(MemoryCell const& a);
        void operator^(MemoryCell const& a);
    };

    struct SHOMInterpreter {
        stack<MemoryCell> Memory;
        
        string File, Line;
        size_t LineNo = 1;

        enum DataType CurrentType = Null;
        string Token = "";

        inline string Trim(string s){
            s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), s.end());
            s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) { return !isspace(ch); }));
            return s;
        }

        inline void Reset(){
            this->CurrentType = Null;
            Token.clear();
        }

        inline void Error(const string& message){
            cout << this->File+":"+to_string(this->LineNo)+": "+message+"\n\t"+this->Line;
            exit(0);
        }

        void Interprete(ifstream& file);
    };
    
    SHOMInterpreter Interpreter;

    void MemoryCell::operator+(MemoryCell const& a){
        if (this->Type==String || a.Type==String)
            STROP(this->Value, a.Value, +);
            
        NUMOPS(+);
    }

    void MemoryCell::operator-(MemoryCell const& a){
        if (this->Type==String)
            Interpreter.Error("Mismatched types");

        NUMOPS(-);
    }

    void MemoryCell::operator*(MemoryCell const& a){
        if (this->Type==String)
            1;

        NUMOPS(*);
    }

    void MemoryCell::operator/(MemoryCell const& a){
        if (this->Type==String)
            1;

        NUMOPS(/);
    }
}