namespace SHOM {
    enum Type {
        Null = 0,
        Integer = 1, 
        Double = 2, 
        String = 3
    };

    struct MemoryCell {
        string Value;
        enum Type Type;

        MemoryCell(string v, enum Type t) : Value(v), Type(t) {}
    };

    struct SHOMInterpreter {
        stack<MemoryCell> Memory;
        
        string File, Line;
        size_t LineNo = 1;

        enum Type CurrentType = Null;

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
}