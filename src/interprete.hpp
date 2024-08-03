namespace SHOM {
    void SHOMInterpreter::Interprete(ifstream& file){
        while (getline(file, this->Line)){
            for (char c : this->Line){
                if (isdigit(c)){
                    if (this->CurrentType!=Double)
                        this->CurrentType = Integer;
                }

                else if (c=='.'){
                    if (this->CurrentType==Double)
                        this->Error("Unexpected floating point");

                    this->CurrentType = Double;
                }

                else {
                    this->Token = Trim(this->Token);

                    if (!this->Token.empty())
                        this->Memory.push(MemoryCell(this->Token, this->CurrentType));

                    if (Syntax::Instructions[c])
                        Syntax::Instructions[c]();

                    Reset();

                    continue;
                }

                this->Token+=c;
            }   

            this->LineNo++;
        }
    }
}