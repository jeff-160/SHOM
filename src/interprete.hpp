namespace SHOM {
    void SHOMInterpreter::Interprete(ifstream& file){
        while (getline(file, this->Line)){
            for (char c : this->Line){
                if (isdigit(c) && this->CurrentType!=String){
                    if (this->CurrentType!=Double)
                        this->CurrentType = Integer;
                }

                else if (c=='"'){
                    if (this->CurrentType!=String)
                        this->CurrentType = String;
                    else
                        goto store;
                }

                else if (this->CurrentType!=String && c=='.'){
                    if (this->CurrentType==Double)
                        this->Error("Unexpected floating point");

                    this->CurrentType = Double;
                }

                else {
                    if (this->CurrentType!=String){
                        this->Token = Trim(this->Token);
                        
                        store:

                        if (this->CurrentType==String)
                            this->Token.erase(this->Token.begin());

                        if (!this->Token.empty())
                            this->Memory.push({this->Token, this->CurrentType});

                        if (Syntax::Instructions[c])
                            Syntax::Instructions[c]();

                        Reset();

                        continue;
                    }
                }

                this->Token+=c;
            }   

            this->LineNo++;
        }
    }
}