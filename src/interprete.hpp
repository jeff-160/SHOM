#define TREELAST(c) (!this->BlockTree.empty() && this->BlockTree.back()==c)
#define INBLOCK TREELAST(Syntax::Braces[0])
#define INSTRING TREELAST(Syntax::Quote)


namespace SHOM {
    void SHOMInterpreter::InterpreteLine(string line){
        line = Trim(line)+" ";

        for (size_t i=0;i<line.size();i++){
            char c = line[i];

            if (this->CurrentType!=String && !INSTRING){
                if (c==Syntax::Braces[0]){
                    this->BlockTree.push_back(c);

                    if (this->BlockTree.size()==1){
                        this->Blocks.push_back("");
                        continue;
                    }
                }

                else if (c==Syntax::Braces[1]){
                    if (this->BlockTree.empty())
                        this->Error("Mismatched brace");
                    else
                        this->BlockTree.pop_back();
                }
            }

            if (INBLOCK || INSTRING){
                this->Blocks.back()+=c;

                if (c==Syntax::Quote && !(i && line[i-1]=='\\')){
                    if (INSTRING)
                        this->BlockTree.pop_back();
                    else
                        this->BlockTree.push_back(Syntax::Quote);
                }

                continue;
            }

            if (isdigit(c) && this->CurrentType!=String){
                if (this->CurrentType!=Double)
                    this->CurrentType = Integer;
            }

            else if (this->CurrentType==String && i<line.size()-1 && c=='\\'){
                if (!Syntax::Escape[line[i+1]])
                    this->Error("Unknown escape sequence");
                
                this->Token+=Syntax::Escape[line[i+1]];
                i++;
                continue;
            }

            else if (c==Syntax::Quote){
                if (this->CurrentType!=String){
                    this->CurrentType = String;
                    goto inc;
                }
                else
                    goto store;
            }

            else if ((c=='+' || c=='-') && (i<this->Line.size()-1 && isdigit(line[i+1])))
                goto inc;

            else if (this->CurrentType!=String && c=='.'){
                if (this->CurrentType==Double)
                    this->Error("Unexpected floating point");

                this->CurrentType = Double;
            }

            else {
                if (this->CurrentType!=String && !INBLOCK){
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
                    else if (!isspace(c) && c!=Syntax::Quote && c!=Syntax::Braces[0] && c!=Syntax::Braces[1])
                        this->Error("Unrecognised instruction");

                    Reset();

                    continue;
                }
            }

            inc:
                this->Token+=c;
        }
        
        if (this->CurrentType==String)
            this->Error("Unterminated string");
    }

    void SHOMInterpreter::Interprete(ifstream& file){
        while (getline(file, this->Line)){
            this->InterpreteLine(this->Line);
            this->LineNo++;
        }

        if (!this->BlockTree.empty())
            this->Error("Unclosed block");
    }
}