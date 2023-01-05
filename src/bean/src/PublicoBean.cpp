/*=========================================================

=========================================================*/
char PublicoBean::write(Table &reg,Table &publico){
  int
    id=reg.getInt("id");

  publico.sort("id");
  if(!id){
    id=publico.go(publico.len()-1).getInt("id");
    reg.putInt("id",id+1);
    publico.addReg(reg.me(),0);
  }
  else{
    int
      x=publico.find("id",Number(id).me());

    if(!FOUND(x)){
      message(MSG_ERROR,"Não achei para atualizar.");
      return 0;
    }
    else{
      publico.go(x);
      publico.updateReg(reg.me(),0);
    }
  }
  publico.sort("nome");
  publico.save();
  return 1;
}
/*=========================================================

=========================================================*/
void PublicoBean::bind(Table &reg){
  BEAN(PhpBind,bind,"app.php.bind")

  bind
    .bind("nome",reg,0,BTRIMONLY)
    .bind("ativo",reg);
}
/*=========================================================

=========================================================*/
char PublicoBean::validate(Table &reg,Table &publico){
  BEAN(PhpValidate,val,"app.php.validate")

  return val
    .zero()
    .setStop(1)
    .setSendError(1)
    .required(reg,"nome")
    .unique(publico,reg,"id","nome")
    .getCount()==0;
}
/*=========================================================

=========================================================*/
void PublicoBean::toResponse(Table &publico){
  BEANMAP(PhpDat,dat,"app.php.dat")
  BEAN(PhpUtil,util,"app.php.util")

  setObj("publicoDat",&publico);
  setStr("id","%d",publico.getInt("id"));
  setStr("nome","%s",util.strNull(publico.getStr("nome")));
  setStr("ativo","%d",publico.getInt("ativo"));
  setStr("publicoReg","%s",dat.serialize(publico).ptr());
}
PublicoBean::PublicoBean(){};
PublicoBean::~PublicoBean(){}
/*=========================================================

=========================================================*/
void PublicoBean::table(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    publico=dat.use("publico").copy();
  String
    row=fileLoad("admin/publico/tableRow.html"),
    exemplo=getStr("exemplo"),
    buffer;
  PhpUtil
    util;

  publico
    .grep(exemplo.me())
    .sort("nome");

  publico.top();
  while(publico.fetch())
    buffer.appendf(
      row.ptr(),
      publico.getInt("id"),
      publico.getStr("nome").ptr(),
      util.format(publico,"ativo").ptr()
    );

  setStr("PublicoTable",buffer);
  dispatch("admin/publico/publico.html");
}
/*=========================================================

=========================================================*/
void PublicoBean::novo(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    publico=dat.use("publico").copy();

  toResponse(publico.removeAll().addNew());
  dispatch("admin/publico/edit.html");
}
/*=========================================================

=========================================================*/
void PublicoBean::select(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    publico=dat.use("publico").copy();
  int
    idPublico=getMap().get("idPublico").asInt(),
    x=publico.sort("id").find("id",Number(idPublico).me());

  if(!FOUND(x)){
    message(MSG_ERROR,"Não achei o que você pediu.");
    novo(map);
  }
  else{
    toResponse(publico.removeAll(x).go(0));
    dispatch("admin/publico/edit.html");
  }
}
/*=========================================================

=========================================================*/
void PublicoBean::save(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    reg=dat.unserialize(getStr("publicoReg"));
  TableRef
    publico=dat.use("publico");

  bind(reg);
  if(validate(reg,publico)&&write(reg,publico))
    message(MSG_SUCCESS,"Realizado com Sucesso.");
  toResponse(reg);
  dispatch("admin/publico/edit.html");
}
/*=========================================================

=========================================================*/
void PublicoBean::remove(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    reg=dat.unserialize(getStr("publicoReg"));
  TableRef
    publico=dat.use("publico");
  int
    id=reg.getInt("id"),
    x;

  x=publico.sort("id").find("id",Number(id).me());
  if(FOUND(x)){
    publico
      .remove(x)
      .sort("nome")
      .save();
    message(MSG_SUCCESS,"Realizado com Sucesso.");
    dispatch("admin/publico/publico.html");
  }
  else{
    publico.sort("nome");
    message(MSG_ERROR,"Não achei [%s] para remover.",reg.getStr("alias").ptr());
    dispatch("admin/publico/publico.html");
  }
}
