/*=========================================================

=========================================================*/
PhpValidate::PhpValidate(){
  initPhpValidate();
}
PhpValidate::~PhpValidate(){
}
void PhpValidate::initPhpValidate(void){
  count=sendError=stop=0;
}
PhpValidate &PhpValidate::setSendError(char sendError){this->sendError=sendError;return *this;}
PhpValidate &PhpValidate::setStop(char stop){this->stop=stop;return *this;}
PhpValidate &PhpValidate::PhpValidate::zero(void){count=0;return *this;}
int PhpValidate::getCount(void){return count;}
/*=========================================================

=========================================================*/
void PhpValidate::showError(cchar *format,...){
  if(sendError){
    message(
      MSG_ERROR,
      "%s",
      String("ERRO: ").append(String().squeeze(cVsprintf(&format))).ptr()
    );
  }
}
/*=========================================================

=========================================================*/
PhpValidate &PhpValidate::unique(Table &table,Table &reg,cchar *pkey,cchar *field){
  if(count&&stop)
    goto FIM;
  else{
    char
      ok;

    ok=table.unique(reg,String(pkey).me(),String(field).me());
    if(!ok){
      count+=1;
      showError(
        "Valor já existe: <u>%s</u> [%s].",
        field,
        reg.getStr(field).ptr()
      );
    }
  }
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
PhpValidate &PhpValidate::required(Table &reg,cchar *field){
  if(count&&stop)
    goto FIM;
  else{
    char
      ok;

    ok=reg.getDef().get(field).getType()==C_TEXT
      ?reg.getStr(field).len()>0
      :reg.getDbl(field)>0;
    if(!ok){
      count+=1;
      showError(
        "Campo obrigatório: <u>%s</u>.",
        field
      );
    }
  }
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
PhpValidate &PhpValidate::cpf(Table &reg,cchar *field,char required){
  if(count&&stop)
    goto FIM;
  else{
    String
      cpf=reg.getStr(field).trim();

    if(!cpf.len()&&!required)
      goto FIM;
    if(!cpfValida(cpf)){
      count+=1;
      showError("CPF Inválido.");
    }
  }
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
PhpValidate &PhpValidate::cnpj(Table &reg,cchar *field,char required){
  if(count&&stop)
    goto FIM;
  else{
    String
      cnpj=reg.getStr(field).trim();

    if(!cnpj.len()&&!required)
      goto FIM;
    if(!cnpjValida(cnpj)){
      count+=1;
      showError("CNPJ Inválido.");
    }
  }
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
PhpValidate &PhpValidate::cpfCnpj(Table &reg,cchar *field,char required){
  if(count&&stop)
    goto FIM;
  else{
    String
      valor=reg.getStr(field).trim();

    switch(valor.len()){
      case 0:
        if(required){
          count+=1;
          showError("CPF/CNPJ obrigatório.");
        }
        break;
      case 11:
        cpf(reg,field,required);
        break;
      case 14:
        cnpj(reg,field,required);
        break;
      default:
        count+=1;
        showError("CPF/CNPJ Inválido.");
    }
  }
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
PhpValidate &PhpValidate::matricula(Table &reg,cchar *field,char required){
  if(count&&stop)
    goto FIM;
  else{
    String
      matricula=reg.getStr(field).trim();

    if(!matricula.len()&&!required)
      goto FIM;
    if(matricula.len()!=8){
      count+=1;
      showError("Matrícula Inválida. 8 dígitos.");
    }
  }
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
PhpValidate &PhpValidate::date(Table &reg,cchar *field,char required){
  if(count&&stop)
    goto FIM;
  else{
    DateFormat
      df;
    Date
      data=reg.getDte(field);

    if(df.isDate(data))
      goto FIM;
    if(!df.isDate(data)){
      data.fromDouble(0);
      if(required){
        count+=1;
        showError("Data Inválida. DD/MM/AAAA.");
        goto FIM;
      }
    }
  }
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
PhpValidate &PhpValidate::id(Table &table,Table &reg,cchar *field){
  Number
    id=reg.getInt(field);

  if(!id.asInt()||(count&&stop))
    goto FIM;
  else{
    int
      x=table.find("id",&id);

    if(!FOUND(x)){
      count+=1;
      showError("Não achei para atualizar.");
      goto FIM;
    }
  }
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
PhpValidate &PhpValidate::combo(Table &reg,cchar *field,cchar *key){
  if(count&&stop)
    goto FIM;
  else{
    if(!key||!*key)
      key=field;
    if(!reg.getInt(field)){
      String
        value=getStr(key);

      count+=1;
      if(!value.len())
        showError(
          "Campo obrigatório: <u>%s</u>.",
          field
        );
      else
        showError(
          "Não achei isto: [%s=%s].",
          field,
          value.ptr()
        );
      goto FIM;
    }
  }
  FIM:
  return *this;
}
