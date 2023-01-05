/*=========================================================

=========================================================*/
char AliasBean::write(Table &reg,Table &alias){
  int
    id=reg.getInt("id");

  alias.sort("id");
  if(!id){
    id=alias.go(alias.len()-1).getInt("id");
    reg.putInt("id",id+1);
    alias.addReg(reg.me(),0);
  }
  else{
    int
      x=alias.find("id",Number(id).me());

    if(!FOUND(x)){
      message(MSG_ERROR,"Não achei para atualizar.");
      return 0;
    }
    else{
      alias.go(x);
      alias.updateReg(reg.me(),0);
    }
  }
  alias.sort("alias");
  alias.save();
  return 1;
}
/*=========================================================

=========================================================*/
void AliasBean::bind(Table &reg){
  BEAN(PhpBind,bind,"app.php.bind")

  bind.bind("alias",reg,0,BTRIMONLY);
  bind.bind("real",reg,0,BTRIMONLY);
  bind.bind("ativo",reg);
}
/*=========================================================

=========================================================*/
char AliasBean::validate(Table &reg,Table &alias){
  BEAN(PhpValidate,val,"app.php.validate")

  return val
    .zero()
    .setStop(1)
    .setSendError(1)
    .required(reg,"alias")
    .required(reg,"real")
    .unique(alias,reg,"id","alias")
    .getCount()==0;
}
/*=========================================================

=========================================================*/
void AliasBean::toResponse(Table &alias){
  BEANMAP(PhpDat,dat,"app.php.dat")
  BEAN(PhpUtil,util,"app.php.util")

  setObj("aliasDat",&alias);
  setStr("id","%d",alias.getInt("id"));
  setStr("alias","%s",util.strNull(alias.getStr("alias")));
  setStr("real","%s",util.strNull(alias.getStr("real").ptr()));
  setStr("ativo","%d",alias.getInt("ativo"));
  setStr("aliasReg","%s",dat.serialize(alias).ptr());
}
AliasBean::AliasBean(){}
AliasBean::~AliasBean(){}
/*=========================================================

=========================================================*/
void AliasBean::table(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    alias=dat.use("alias").copy();
  String
    row=fileLoad("admin/alias/tableRow.html"),
    exemplo=getStr("exemplo"),
    buffer;
  PhpUtil
    util;

  alias
    .grep(exemplo.me())
    .sort("alias");

  alias.top();
  while(alias.fetch())
    buffer.appendf(
      row.ptr(),
      alias.getInt("id"),
      alias.getStr("alias").ptr(),
      alias.getStr("real").ptr(),
      util.format(alias,"ativo").ptr()
    );

  setStr("AliasTable",buffer);
  dispatch("admin/alias/alias.html");
}
/*=========================================================

=========================================================*/
void AliasBean::novo(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    alias=dat.use("alias").copy();

  toResponse(alias.removeAll().addNew());
  dispatch("admin/alias/edit.html");
}
/*=========================================================

=========================================================*/
void AliasBean::select(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    alias=dat.use("alias").copy();
  int
    idAlias=getMap().get("idAlias").asInt(),
    x=alias.sort("id").find("id",Number(idAlias).me());

  if(!FOUND(x)){
    message(MSG_ERROR,"Não achei o que você pediu.");
    novo(map);
  }
  else{
    toResponse(alias.removeAll(x).go(0));
    dispatch("admin/alias/edit.html");
  }
}
/*=========================================================

=========================================================*/
void AliasBean::reuse(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    alias=dat.use("alias").copy();
  int
    idAlias=getMap().get("idAlias").asInt(),
    x=alias.sort("id").find("id",Number(idAlias).me());

  if(!FOUND(x)){
    message(MSG_ERROR,"Não achei o que você pediu.");
    dispatch("admin/alias/alias.html");
  }
  else{
    alias.removeAll(x).go(0);
    alias.putInt("id",0);
    toResponse(alias);
    dispatch("admin/alias/edit.html");
  }
}
/*=========================================================

=========================================================*/
void AliasBean::save(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    reg=dat.unserialize(getStr("aliasReg"));
  TableRef
    alias=dat.use("alias");

  bind(reg);
  if(validate(reg,alias)&&write(reg,alias))
    message(MSG_SUCCESS,"Realizado com Sucesso.");
  toResponse(reg);
  dispatch("admin/alias/edit.html");
}
/*=========================================================

=========================================================*/
void AliasBean::import(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  TableRef
    alias=dat.use("alias");
  ByteArray
    buffer=getMap().get("csv").asStr().ptr();
  String
    linha;
  Table
    reg=alias.copy().removeAll().addNew();
  int
    last=alias.sort("id").go(alias.len()-1).getInt("id"),
    a=0,
    u=0;

  alias.sort("alias");
  buffer.seek(0);
  while(buffer.readLine(linha)){
    Split
      item=Split(linha,';');
    int
      x;

    if(!linha.trim().len())
      continue;
    reg
      .putStr("id",item[0].trim())
      .putStr("alias",item[1].trim())
      .putStr("real",item[2].trim())
      .putStr("ativo",item[3].trim());

    x=alias.find("alias",reg.getStr("alias").me());
    if(FOUND(x)){
      alias.go(x);
      reg.putInt("id",alias.getInt("id"));
      alias.updateReg(reg.me(),0);
      u+=1;
    }
    else{
      reg.putInt("id",last+=1);
      alias
        .addReg(reg.me(),0)
        .sort("alias");
      a+=1;
    }
  }
  alias.save();
  printf("Realizado com Sucesso. add=[%d] upd=[%d].",a,u);
}
/*=========================================================

=========================================================*/
void AliasBean::remove(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    reg=dat.unserialize(getStr("aliasReg"));
  TableRef
    alias=dat.use("alias");
  int
    id=reg.getInt("id"),
    x;

  x=alias.sort("id").find("id",Number(id).me());
  if(FOUND(x)){
    alias
      .remove(x)
      .sort("alias")
      .save();
    message(MSG_SUCCESS,"Realizado com Sucesso.");
    dispatch("admin/alias/alias.html");
  }
  else{
    alias.sort("alias");
    message(MSG_ERROR,"Não achei [%s] para remover.",reg.getStr("alias").ptr());
    dispatch("admin/alias/alias.html");
  }
}
