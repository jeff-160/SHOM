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
                        this->Error("Mismatched block brace");
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

            if (c==Syntax::Array[0]){
                if (this->InArray)
                    this->Error("Unexpected array brace");
                this->InArray = true;
            }

            if (isdigit(c) && this->CurrentType!=String){
                if (this->CurrentType!=Double)
                    this->CurrentType = Integer;
            }

            else if (this->CurrentType==String && i<line.size()-1 && c=='\\'){
                if (!Syntax::Escape[line[i+1]])
                    this->Error("Unknown escape sequence: \\", string(1, c));
                
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
                        MemoryCell m;

                        switch (this->CurrentType){
                            case Integer:
                                m = {stoll(this->Token), this->CurrentType};
                                break;
                            case Double:
                                m = {stod(this->Token), this->CurrentType};
                                break;
                            case String:
                                this->Token.erase(this->Token.begin());
                                m = {this->Token, this->CurrentType};
                                break;
                        }

                        this->InArray ? this->CurrentArray.push_back(m) : this->Memory.push(m);
                    }
                    
                    if (c==Syntax::Array[1]){
                        this->InArray = false;

                        this->Memory.push({"", Array, this->CurrentArray});
                        this->CurrentArray.clear();
                    }

                    if (Syntax::Instructions[c]){
                        if (this->InArray)
                            this->Error("Unexpected instruction: ", string(1, c));
                        Syntax::Instructions[c]();
                    }
                    else if (!isspace(c) && c!=Syntax::Quote && c!=Syntax::Braces[0] && c!=Syntax::Braces[1] && c!=Syntax::Array[0] && c!=Syntax::Array[1])
                        this->Error("Unrecognised instruction: ", string(1, c));

                    Reset();

                    continue;
                }
            }

            inc:
                this->Token+=c;
        }
        
        if (this->CurrentType==String)
            this->Error("Unterminated string");
        if (this->InArray)
            this->Error("Unclosed array");
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