namespace SHOM {
    void SHOMInterpreter::Interprete(ifstream& file){
        while (getline(file, this->Line)){
            for (size_t i=0;i<this->Line.size();i++){
                char c = this->Line[i];

                if (isdigit(c) && this->CurrentType!=String){
                    if (this->CurrentType!=Double)
                        this->CurrentType = Integer;
                }

                else if (this->CurrentType==String && i<this->Line.size()-1 && c=='\\'){
                    if (!Syntax::Escape[this->Line[i+1]])
                        Error("Unknown escape sequence");
                    
                    this->Token+=Syntax::Escape[this->Line[i+1]];
                    i++;
                    continue;
                }

                else if (c=='"'){
                    if (this->CurrentType!=String)
                        this->CurrentType = String;
                    else
                        goto store;
                }

                else if ((c=='+' || c=='-') && (i<this->Line.size()-1 && isdigit(this->Line[i+1])))
                    goto inc;

                else if (this->CurrentType!=String && c=='.'){
                    if (this->CurrentType==Double)
                        this->Error("Unexpected floating point");

                    this->CurrentType = Double;
                }

                else {
                    if (this->CurrentType!=String){
                        this->Token = Trim(this->Token);
                        
                        store:

                        if (!this->Token.empty()){
                            switch (this->CurrentType){
                                case Integer:
                                    this->Memory.push({stoll(this->Token), this->CurrentType});
                                    break;
                                case Double:
                                    this->Memory.push({stod(this->Token), this->CurrentType});
                                    break;
                                case String:
                                    this->Token.erase(this->Token.begin());
                                    this->Memory.push({this->Token, this->CurrentType});
                                    break;
                            }
                        }

                        if (Syntax::Instructions[c])
                            Syntax::Instructions[c]();

                        Reset();

                        continue;
                    }
                }

                inc:
                    this->Token+=c;
            }   

            if (this->CurrentType==String)
                Error("Unterminated string");

            this->LineNo++;
        }
    }
}