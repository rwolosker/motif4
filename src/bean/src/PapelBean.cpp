/*=========================================================

=========================================================*/
char PapelBean::write(Table &reg,Table &papel){
  int
    id=reg.getInt("id");

  papel.sort("id");
  if(!id){
    id=papel.go(papel.len()-1).getInt("id");
    reg.putInt("id",id+1);
    papel.addReg(reg.me(),0);
  }
  else{
    int
      x=papel.find("id",Number(id).me());

    if(!FOUND(x)){
      message(MSG_ERROR,"Não achei para atualizar.");
      return 0;
    }
    else{
      papel.go(x);
      papel.updateReg(reg.me(),0);
    }
  }
  papel.save();
  return 1;
}
/*=========================================================

=========================================================*/
void PapelBean::bind(Table &reg){
  BEAN(PhpBind,bind,"app.php.bind")

  bind.bind("nome",reg,0,BTRIMONLY);
}
/*=========================================================

=========================================================*/
char PapelBean::validate(Table &reg,Table &papel){
  BEAN(PhpValidate,val,"app.php.validate")

  return val
    .zero()
    .setStop(1)
    .setSendError(1)
    .required(reg,"nome")
    .unique(papel,reg,"id","nome")
    .getCount()==0;
}
/*=========================================================

=========================================================*/
void PapelBean::toResponse(Table &papel){
  BEANMAP(PhpDat,dat,"app.php.dat")
  BEAN(PhpUtil,util,"app.php.util")

  setObj("papelDat",&papel);
  setStr("id","%d",papel.getInt("id"));
  setStr("nome","%s",util.strNull(papel.getStr("nome")));
  setStr("papelReg","%s",dat.serialize(papel).ptr());
}
PapelBean::PapelBean(){};
PapelBean::~PapelBean(){}
/*=========================================================

=========================================================*/
void PapelBean::table(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    papel=dat.use("papel").copy();
  String
    row=fileLoad("admin/papel/tableRow.html"),
    exemplo=getStr("exemplo"),
    buffer;
  PhpUtil
    util;

  papel
    .grep(exemplo.me())
    .sort("nome");

  papel.top();
  while(papel.fetch())
    buffer.appendf(
      row.ptr(),
      papel.getInt("id"),
      papel.getStr("nome").ptr()
    );

  setStr("PapelTable",buffer);
  dispatch("admin/papel/papel.html");
}
/*=========================================================

=========================================================*/
void PapelBean::novo(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    papel=dat.use("papel");

  toResponse(papel.removeAll().addNew());
  dispatch("admin/papel/edit.html");
}
/*=========================================================

=========================================================*/
void PapelBean::select(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    papel=dat.use("papel").copy();
  int
    idPapel=getMap().get("idPapel").asInt(),
    x=papel.find("id",Number(idPapel).me());

  if(!FOUND(x)){
    message(MSG_ERROR,"Não achei o que você pediu.");
    novo(map);
  }
  else{
    toResponse(papel.removeAll(x).go(0));
    dispatch("admin/papel/edit.html");
  }
}
/*=========================================================

=========================================================*/
void PapelBean::save(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    reg=dat.unserialize(getStr("papelReg"));
  TableRef
    papel=dat.use("papel");

  bind(reg);
  if(validate(reg,papel)&&write(reg,papel))
    message(MSG_SUCCESS,"Realizado com Sucesso.");
  toResponse(reg);
  dispatch("admin/papel/edit.html");
}
/*=========================================================

=========================================================*/
void PapelBean::methods(Map &map){
  Split
    list=getFactory().getReflection().getMap().keys();
  ByteArray
    buffer;
  PhpUtil
    util;

  list.sort();
  for(int i=0;i<list.len();i++)
    if(!util.isHexNumber(list[i])&&!list[i].equals("#passed"))
      buffer.writeStr(list[i]).writeStr("\n");

  printf("%s",buffer.toString().ptr());
}
/*=========================================================

=========================================================*/
void PapelBean::import(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  TableRef
    papel=dat.use("papel");
  ByteArray
    buffer=ByteArray(getStr("txt"));
  String
    linha;
  int
    id;
  Split
    novo;

  papel.sort("nome");
  buffer.seek(0);
  while(buffer.readLine(linha))
    if((linha=linha.trim()).len()){
      int
        x=papel.find("nome",&linha);

      if(!FOUND(x))
        novo.add(linha);
    }
  id=papel.sort("id").go(papel.len()-1).getBln("id");
  for(int i=0;i<novo.len();i++)
    papel
      .addNew()
      .putInt("id",id+=1)
      .putStr("nome",novo[i]);
  papel.sort("id");
  if(novo.len())
    papel.save();
  printf("Realizado com Sucesso. a=[%d].",novo.len());
}
/*=========================================================

=========================================================*/
void PapelBean::remove(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    reg=dat.unserialize(getStr("papelReg"));
  TableRef
    papel=dat.use("papel"),
    grupoPapel=dat.use("grupoPapel");
  int
    id=reg.getInt("id"),
    x;

  x=grupoPapel.copy().sort("papel").find("papel",Number(id).me());
  if(FOUND(x)){
    message(MSG_ERROR,"Este papel está em uso em [GrupoPapel].");
    dispatch("admin/papel/edit.html");
  }
  else{
    x=papel.find("id",Number(id).me());
    if(FOUND(x)){
      papel
        .remove(x)
        .save();
      message(MSG_SUCCESS,"Realizado com Sucesso.");
      dispatch("admin/papel/papel.html");
    }
    else{
      message(MSG_ERROR,"Não achei [%s] para remover.",reg.getStr("nome").ptr());
      dispatch("admin/papel/papel.html");
    }
  }
}
/*=========================================================

=========================================================*/
void PapelBean::configExport(Map &map){
  ConfigBackup
    config;

  inject(&config);
  printf("%s",config.backup().ptr());
}
/*=========================================================

=========================================================*/
void PapelBean::configImport(Map &map){
  Split
    opcao=Split(getStr("opcao"),',');
  ByteArray
    buffer=ByteArray(getStr("txt").ptr());
  ConfigBackup
    config;
  String
    res;

  inject(&config);
  res=config.restore("",buffer);
  printf("%s",res.ptr());
}

