#define ETERR(t) \
    if (this->Type==t || a.Type==t) throw 0

#define ARRERR \
    if ((this->Type==Array && a.Type!=Array) || (this->Type!=Array && a.Type==Array)) throw 0;

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
    if constexpr (is_same_v<T, string>) { \
        ostringstream oss; \
        oss << setprecision(16) << v; \
        return oss.str(); \
    } \
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
        String,
        Array
    };

    struct MemoryCell {
        variant<long long, double, string> Value;
        DataType Type;
        vector<MemoryCell> ArrayValue;

        template <typename T>
        T Cast() const;

        size_t Size() const {
            switch (this->Type){
                case Integer:
                    return this->Cast<long long>();
                case Double:
                    return this->Cast<double>();
                case String:
                    return this->Cast<string>().size();
                case Array:
                    return this->ArrayValue.size();
            }
        }

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

        bool InArray = false;
        vector<MemoryCell> CurrentArray;

        deque<string> Blocks;
        vector<char> BlockTree;

        long long Iterator = -1;

        inline string Trim(string s){
            s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), s.end());
            s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) { return !isspace(ch); }));
            return s;
        }

        inline void Reset(){
            this->CurrentType = Null;
            Token.clear();
        }

        inline void Error(const string& message, string spec=""){
            cout << endl << this->File+":"+to_string(this->LineNo)+": "+message+spec+"\n\t"+this->Line;
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
        else if (this->Type==Array){
            if constexpr (is_same_v<T, string>){
                string s = "[";
                for (size_t i=0;i<this->ArrayValue.size();i++)
                    s+=this->ArrayValue[i].Cast<string>()+(i<this->ArrayValue.size()-1 ? " " : "");
                return s+"]";
            }
            else if constexpr (is_same_v<T, bool>)
                return !this->ArrayValue.empty();
            else 
                throw 0;
        }
        else {
            auto v = get<string>(this->Value);
            
            if constexpr (is_same_v<T, long long>)
                return stoll(v);
            else if constexpr (is_same_v<T, double>)
                return stod(v);
            else if constexpr (is_same_v<T, bool>)
                return !v.empty();
            else
                return v;
        }
    }

    void MemoryCell::operator+(MemoryCell const& a){
        ARRERR;
        
        if (this->Type==String || a.Type==String)
            Interpreter.Memory.push({this->Cast<string>()+a.Cast<string>(), String});

        if (this->Type==Array && a.Type==Array){
            for (MemoryCell i : a.ArrayValue)
                this->ArrayValue.push_back(i);
            Interpreter.Memory.push({"", Array, this->ArrayValue});
        }
            
        NUMOPS(+);
    }

    void MemoryCell::operator-(MemoryCell const& a){
        ETERR(String);
        ETERR(Array);
        NUMOPS(-);
    }

    void MemoryCell::operator*(MemoryCell const& a){
        if (this->Type==Array && a.Type==Array)
            throw 0;

        MemoryCell x = *this, y = a;
        if (y.Type==String)
            swap(x, y);  
        if (x.Type==String)
            Interpreter.Memory.push({x.Cast<string>()*y.Cast<long long>(), String});

        NUMOPS(*);
    }

    void MemoryCell::operator/(MemoryCell const& a){
        ETERR(String);
        ETERR(Array);

        if (!a.Cast<long long>())
            Interpreter.Error("Division by 0");
        NUMOPS(/);
    }

    void MemoryCell::operator&(MemoryCell const& a){
        Interpreter.Memory.push({this->Cast<bool>() && a.Cast<bool>(), Integer});
    }

    void MemoryCell::operator|(MemoryCell const& a){
        Interpreter.Memory.push({this->Cast<bool>() || a.Cast<bool>(), Integer});
    }
    
    void MemoryCell::operator==(MemoryCell const& a){
        Interpreter.Memory.push({this->Type!=a.Type ? 0 : this->Cast<string>()==a.Cast<string>(), Integer});
    }
    
    void MemoryCell::operator!=(MemoryCell const& a){
        Interpreter.Memory.push({this->Type!=a.Type ? 1 : this->Cast<string>()!=a.Cast<string>(), Integer});
    }

    void MemoryCell::operator>(MemoryCell const& a){
        Interpreter.Memory.push({this->Size() > a.Size(), Integer});
    }
    
    void MemoryCell::operator<(MemoryCell const& a){
        Interpreter.Memory.push({this->Size() < a.Size(), Integer});
    }
    
    void MemoryCell::operator^(MemoryCell const& a){
        ETERR(Array);

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
        ETERR(String);
        ETERR(Array);

        else if(this->Type==Integer)
            NUMOP(this, a, %, long long, Integer);
        
        else if(this->Type==Double){
            double x = this->Cast<double>(), y = a.Cast<double>();   
            Interpreter.Memory.push({x-(int)(x/y)*y, Double});
        }
    }
}