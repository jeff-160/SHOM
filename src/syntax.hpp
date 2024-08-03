namespace SHOM {
    namespace Syntax {
        char Quote = '"';

        unordered_map<char, function<void()>> Instructions = {
            {'~', [](){
                cout << (Interpreter.Memory.size() ? Interpreter.Memory.top().Value : "");
            }},

            {';', [](){
                if (!Interpreter.Memory.empty())
                    Interpreter.Memory.pop();
            }},

            {'_', [](){
                while (!Interpreter.Memory.empty())
                    Interpreter.Memory.pop();
            }},

            {'+', [](){
                unsigned long long a = stoull(Interpreter.Memory.top().Value);
                Interpreter.Memory.pop();
                unsigned long long b = stoull(Interpreter.Memory.top().Value);
                Interpreter.Memory.pop();

                Interpreter.Memory.push(MemoryCell(to_string(a+b), Integer));
            }}
        };
    }
}