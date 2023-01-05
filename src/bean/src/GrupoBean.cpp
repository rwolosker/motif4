/*=========================================================

=========================================================*/
char GrupoBean::write(Table &reg,Table &grupo){
  int
    id=reg.getInt("id");

  grupo.sort("id");
  if(!id){
    id=grupo.go(grupo.len()-1).getInt("id");
    reg.putInt("id",id+1);
    grupo.addReg(reg.me(),0);
  }
  else{
    int
      x=grupo.find("id",Number(id).me());

    if(!FOUND(x)){
      message(MSG_ERROR,"Não achei para atualizar.");
      return 0;
    }
    else{
      grupo.go(x);
      grupo.updateReg(reg.me(),0);
    }
  }
  grupo.save();
  return 1;
}
/*=========================================================

=========================================================*/
void GrupoBean::bind(Table &reg){
  BEAN(PhpBind,bind,"app.php.bind")

  bind.bind("nome",reg,0,BTRIMONLY);
}
/*=========================================================

=========================================================*/
char GrupoBean::validate(Table &reg,Table &grupo){
  BEAN(PhpValidate,val,"app.php.validate")

  return val
    .zero()
    .setStop(1)
    .setSendError(1)
    .required(reg,"nome")
    .unique(grupo,reg,"id","nome")
    .getCount()==0;
}
/*=========================================================

=========================================================*/
void GrupoBean::toResponse(Table &grupo){
  BEANMAP(PhpDat,dat,"app.php.dat")
  BEAN(PhpUtil,util,"app.php.util")

  setObj("grupoDat",&grupo);
  setStr("id","%d",grupo.getInt("id"));
  setStr("nome","%s",util.strNull(grupo.getStr("nome")));
  setStr("grupoReg","%s",dat.serialize(grupo).ptr());
}
GrupoBean::GrupoBean(){};
GrupoBean::~GrupoBean(){}
/*=========================================================

=========================================================*/
void GrupoBean::table(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    grupo=dat.use("grupo").copy();
  String
    row=fileLoad("admin/grupo/tableRow.html"),
    exemplo=getStr("exemplo"),
    buffer;
  PhpUtil
    util;

  grupo
    .grep(exemplo.me())
    .sort("nome");

  grupo.top();
  while(grupo.fetch())
    buffer.appendf(
      row.ptr(),
      grupo.getInt("id"),
      grupo.getStr("nome").ptr()
    );

  setStr("GrupoTable",buffer);
  dispatch("admin/grupo/grupo.html");
}
/*=========================================================

=========================================================*/
void GrupoBean::novo(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    grupo=dat.use("grupo").copy();

  toResponse(grupo.removeAll().addNew());
  dispatch("admin/grupo/edit.html");
}
/*=========================================================

=========================================================*/
void GrupoBean::select(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    grupo=dat.use("grupo").copy();
  int
    idGrupo=getMap().get("idGrupo").asInt(),
    x=grupo.find("id",Number(idGrupo).me());

  if(!FOUND(x)){
    message(MSG_ERROR,"Não achei o que você pediu.");
    novo(map);
  }
  else{
    toResponse(grupo.removeAll(x).go(0));
    dispatch("admin/grupo/edit.html");
  }
}
/*=========================================================

=========================================================*/
void GrupoBean::save(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    reg=dat.unserialize(getStr("grupoReg"));
  TableRef
    grupo=dat.use("grupo");

  bind(reg);
  if(validate(reg,grupo)&&write(reg,grupo))
    message(MSG_SUCCESS,"Realizado com Sucesso.");
  toResponse(reg);
  dispatch("admin/grupo/edit.html");
}
/*=========================================================

=========================================================*/
void GrupoBean::remove(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    reg=dat.unserialize(getStr("grupoReg")),
    usuarioGrupo=dat.use("usuarioGrupo"),
    grupoPapel=dat.use("grupoPapel");
  TableRef
    grupo=dat.use("grupo");
  Number
    id=reg.getInt("id");
  int
    xg=grupo.sort("id").find("id",&id),
    xug=usuarioGrupo.sort("grupo").find("grupo",&id),
    xgp=grupoPapel.sort("grupo").find("grupo",&id);

  if(FOUND(xug)){
    message(MSG_ERROR,"Não posso. está em uso por [UsuarioGrupo].");
    dispatch("admin/grupo/edit.html");
    return;
  }
  if(FOUND(xgp)){
    message(MSG_ERROR,"Não posso. está em uso por [GrupoPapel].");
    dispatch("admin/grupo/edit.html");
    return;
  }
  if(!FOUND(xg)){
    message(MSG_ERROR,"Não achei [%s] para remover.",reg.getStr("alias").ptr());
    dispatch("admin/grupo/grupo.html");
    return;
  }
  grupo
    .remove(xg)
    .save();
  message(MSG_SUCCESS,"Realizado com Sucesso.");
  dispatch("admin/grupo/grupo.html");
}
