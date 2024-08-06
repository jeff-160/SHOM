#define SERR \
    if (this->Type==String || a.Type==String) throw 0

#define NUMOP(a, b, o, f, t) \
    Interpreter.Memory.push({(a->Cast<f>() o b.Cast<f>()), t})

#define NUMOPS(o) \
    else if (this->Type==Integer) { \
        NUMOP(this, a, o, long long, Integer); \
    } \
    else if (this->Type==Double) { \
        NUMOP(this, a, o, double, Double); \
    }

#define LOGOP(x, y, o) \
    if (a.Type==String || this->Type==String){ \
        Interpreter.Memory.push({(x o y), Integer}); \
    } \
    NUMOPS(o);

#define IMPCAST \
    if constexpr (is_same_v<T, string>) return to_string(v); \
    else return (T)v; \


string operator*(const string& s, long long n){
    string r = "";
    for (long long i=0;i<n;i++)
        r+=s;
    return r;
}

namespace SHOM {
    enum DataType {
        Null,
        Integer,
        Double,
        String
    };

    struct MemoryCell {
        variant<long long, double, string> Value;
        DataType Type;

        template <typename T>
        T Cast() const;

        void operator+(MemoryCell const& a);
        void operator-(MemoryCell const& a);
        void operator*(MemoryCell const& a);
        void operator/(MemoryCell const& a);
        void operator|(MemoryCell const& a);
        void operator&(MemoryCell const& a);
        void operator==(MemoryCell const& a);
        void operator!=(MemoryCell const& a);
        void operator>(MemoryCell const& a);
        void operator<(MemoryCell const& a);
        void operator^(MemoryCell const& a);
        void operator%(MemoryCell const& a);
    };

    struct SHOMInterpreter {
        stack<MemoryCell> Memory;
        
        string File, Line;
        size_t LineNo = 1;

        enum DataType CurrentType = Null;
        string Token = "";

        deque<string> Blocks;
        vector<char> BlockTree;

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
            cout << endl << this->File+":"+to_string(this->LineNo)+": "+message+"\n\t"+this->Line;
            exit(0);
        }

        void Interprete(ifstream& file);
        void InterpreteLine(string line);
    };
    
    SHOMInterpreter Interpreter;

    template <typename T>
    T MemoryCell::Cast() const {
        if (this->Type==Integer){
            auto v = get<long long>(this->Value);
            IMPCAST;
        }
        else if (this->Type==Double){
            auto v = get<double>(this->Value);
            IMPCAST;
        }
        else {
            auto v = get<string>(this->Value);
            
            if constexpr (is_same_v<T, long long>)
                return stoll(v);
            else if constexpr (is_same_v<T, double>)
                return stod(v);
            else
                return v;
        }
    }

    void MemoryCell::operator+(MemoryCell const& a){
        if (this->Type==String || a.Type==String)
            Interpreter.Memory.push({this->Cast<string>()+a.Cast<string>(), String});
            
        NUMOPS(+);
    }

    void MemoryCell::operator-(MemoryCell const& a){
        SERR;
        NUMOPS(-);
    }

    void MemoryCell::operator*(MemoryCell const& a){
        if (a.Type==String)
            Interpreter.Memory.push({a.Cast<string>()*this->Cast<long long>(), String});

        else if (this->Type==String)
            Interpreter.Memory.push({this->Cast<string>()*a.Cast<long long>(), String});

        NUMOPS(*);
    }

    void MemoryCell::operator/(MemoryCell const& a){
        if (!a.Cast<long long>())
            Interpreter.Error("Division by 0");

        SERR;
        NUMOPS(/);
    }

    void MemoryCell::operator&(MemoryCell const& a){
        LOGOP(!this->Cast<string>().empty(), !a.Cast<string>().empty(), &&);
    }

    void MemoryCell::operator|(MemoryCell const& a){
        LOGOP(!this->Cast<string>().empty(), !a.Cast<string>().empty(), ||);
    }
    
    void MemoryCell::operator==(MemoryCell const& a){
        LOGOP(this->Cast<string>(), a.Cast<string>(), ==);
    }
    
    void MemoryCell::operator!=(MemoryCell const& a){
        LOGOP(this->Cast<string>(), a.Cast<string>(), !=);
    }

    void MemoryCell::operator>(MemoryCell const& a){
        LOGOP(this->Cast<string>().size(), a.Cast<string>().size(), >);
    }
    
    void MemoryCell::operator<(MemoryCell const& a){
        LOGOP(this->Cast<string>().size(), a.Cast<string>().size(), <);
    }
    
    void MemoryCell::operator^(MemoryCell const& a){
        if (this->Type==String || a.Type==String)
            throw 0;
        
        else if (this->Type==Integer){
            long long x = this->Cast<long long>(), r = 1;

            for (long long i=0;i<a.Cast<long long>();i++)
                r*=x;

            Interpreter.Memory.push({r, Integer});
        }
        
        else if (this->Type==Double)
            Interpreter.Memory.push({pow(this->Cast<double>(), a.Cast<double>()), Double});
    }

    void MemoryCell::operator%(MemoryCell const& a){
        SERR;

        else if(this->Type==Integer)
            NUMOP(this, a, %, long long, Integer);
        
        else if(this->Type==Double){
            double x = this->Cast<double>(), y = a.Cast<double>();   
            Interpreter.Memory.push({x-(int)(x/y)*y, Double});
        }
    }
}