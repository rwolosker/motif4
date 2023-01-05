/*=========================================================

=========================================================*/
char UsuarioBean::write(Table &reg,Table &usuario){
  int
    id=reg.getInt("id");

  usuario.sort("id");
  if(!id){
    id=usuario.go(usuario.len()-1).getInt("id");
    reg.putInt("id",id+1);
    usuario.addReg(reg.me(),0);
  }
  else{
    int
      x=usuario.find("id",Number(id).me());

    if(!FOUND(x)){
      message(MSG_ERROR,"Não achei para atualizar.");
      return 0;
    }
    else{
      usuario.go(x);
      usuario.updateReg(reg.me(),0);
    }
  }
  usuario.save();
  return 1;
}
/*=========================================================

=========================================================*/
void UsuarioBean::bind(Table &reg){
  BEAN(PhpBind,bind,"app.php.bind")

  bind
    .bind("nome",reg,0,BCAPITALIZE)
    .bind("matricula",reg,0,BINTEGER)
    .bind("cpf",reg,0,BINTEGER)
    .bind("telefone",reg,0,BINTEGER)
    .bind("email",reg,0,BTRIMONLY)
    .bind("ativo",reg)
    .bind("bloqueado",reg)
    .bind("ldap",reg)
    .bind("cerberus",reg);
}
/*=========================================================

=========================================================*/
char UsuarioBean::validate(Table &reg,Table &usuario){
  BEAN(PhpValidate,val,"app.php.validate")
  int
    ok;

  val
  .setStop(1)
  .setSendError(1);

  ok=val
    .zero()
    .required(reg,"nome")
    .required(reg,"email")
    .cpf(reg,"cpf",0)
    .matricula(reg,"matricula",0)
    .unique(usuario,reg,"id","nome")
    .unique(usuario,reg,"id","email")
    .getCount();

  if(ok)
    return 0;

  if(reg.getStr("cpf").len()){
    ok=val
      .zero()
      .unique(usuario,reg,"id","cpf")
      .getCount();
    if(ok)
      return 0;
  }
  if(reg.getStr("matricula").len()){
    ok=val
      .zero()
      .unique(usuario,reg,"id","matricula")
      .getCount();
    if(ok)
      return 0;
  }
  if(!reg.getStr("matricula").len()&&!reg.getStr("cpf").len()){
    message(MSG_ERROR,"Informe pelo menos matrícula ou CPF.");
    return 0;
  }
  return 1;
}
/*=========================================================

=========================================================*/
void UsuarioBean::toResponse(Table &usuario){
  BEANMAP(PhpDat,dat,"app.php.dat")
  BEAN(PhpUtil,util,"app.php.util")

  setObj("usuarioDat",&usuario);
  setStr("id","%d",usuario.getInt("id"));
  setStr("nome","%s",util.strNull(usuario.getStr("nome")));
  setStr("matricula","%s",util.strNull(usuario.getStr("matricula")));
  setStr("cpf","%s",util.strNull(usuario.getStr("cpf")));
  setStr("email","%s",util.strNull(usuario.getStr("email")));
  setStr("ativo","%d",(usuario.getInt("ativo")));
  setStr("bloqueado","%d",(usuario.getInt("bloqueado")));
  setStr("cerberus","%d",(usuario.getInt("cerberus")));
  setStr("ldap","%d",(usuario.getInt("ldap")));
  setStr("telefone","%s",util.strNull(usuario.getStr("telefone")));
  setStr("usuarioReg","%s",dat.serialize(usuario).ptr());
}
UsuarioBean::UsuarioBean(){};
UsuarioBean::~UsuarioBean(){}
/*=========================================================

=========================================================*/
void UsuarioBean::table(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    usuario=dat.use("usuario").copy();
  String
    row=fileLoad("admin/usuario/tableRow.html"),
    exemplo=getStr("exemplo"),
    buffer;
  PhpUtil
    util;

  usuario
    .grep(exemplo.me())
    .sort("nome");

  usuario.top();
  while(usuario.fetch())
    buffer.appendf(
      row.ptr(),
      usuario.getInt("id"),
      usuario.getStr("nome").ptr()
    );

  setStr("UsuarioTable",buffer);
  dispatch("admin/usuario/usuario.html");
}
/*=========================================================

=========================================================*/
void UsuarioBean::novo(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    usuario=dat.use("usuario").copy();

  toResponse(usuario.removeAll().addNew());
  dispatch("admin/usuario/edit.html");
}
/*=========================================================

=========================================================*/
void UsuarioBean::select(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    usuario=dat.use("usuario").copy();
  int
    idUsuario=getMap().get("idUsuario").asInt(),
    x=usuario.find("id",Number(idUsuario).me());

  if(!FOUND(x)){
    message(MSG_ERROR,"Não achei o que você pediu.");
    novo(map);
  }
  else{
    toResponse(usuario.removeAll(x).go(0));
    dispatch("admin/usuario/edit.html");
  }
}
/*=========================================================

=========================================================*/
void UsuarioBean::save(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    reg=dat.unserialize(getStr("usuarioReg"));
  TableRef
    usuario=dat.use("usuario");

  bind(reg);
  if(validate(reg,usuario)&&write(reg,usuario))
    message(MSG_SUCCESS,"Realizado com Sucesso.");
  toResponse(reg);
  dispatch("admin/usuario/edit.html");
}
/*=========================================================

=========================================================*/
char UsuarioBean::byUsuarioGrupoPapel(TableSort &sort,void *a,void *b){
  String
    una=sort.getField(a,"usuario.nome").getString(),
    unb=sort.getField(b,"usuario.nome").getString(),
    gna=sort.getField(a,"grupo.nome").getString(),
    gnb=sort.getField(b,"grupo.nome").getString(),
    pna=sort.getField(a,"papel.nome").getString(),
    pnb=sort.getField(b,"papel.nome").getString();
  char
    u=una.compare(unb),
    g=gna.compare(gnb),
    p=pna.compare(pnb);

  return u?u:g?g:p;
}
/*=========================================================

=========================================================*/
void UsuarioBean::permissaoList(Map &map){
  String
    row=fileLoad("admin/usuario/permissaoRow.html"),
    exemplo=getStr("exemplo"),
    buffer;
  PermissaoList
    list;
  Table
    table;

  inject(&list);
  list.list(table);
  if(exemplo.len())
    table.grep(&exemplo);
  table.sort(
    this,METHOD(&UsuarioBean::byUsuarioGrupoPapel)
  );
  table.top();
  while(table.fetch())
    buffer.appendf(
      row,
      table.getStr("usuario.matricula").ptr(),
      table.getStr("usuario.nome").ptr(),
      table.getStr("grupo.nome").ptr(),
      table.getStr("papel.nome").ptr()
    );

  setStr("PermissaoTable",buffer);
  dispatch("admin/usuario/permissao.html");
}
/*=========================================================

=========================================================*/
void UsuarioBean::senhaDefine(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  TableRef
    usuarioSenha=dat.use("usuarioSenha");
  Number
    id=getInt("id");
  String
    senha0=getStr("senha0"),
    senha1=getStr("senha0");

  if(!senha0.equals(senha1))
    printf("ERRO: Senha não é igual.");
  else{
    int
      x=usuarioSenha.find("id",&id);

    if(FOUND(x))
      usuarioSenha.go(x).putStr("senha",senha0);
    else
      usuarioSenha
        .addNew()
        .putInt("id",id.asInt())
        .putStr("senha",senha0)
        .sort("id");

    usuarioSenha.save();
    printf("Realizado com Sucesso.");
  }
}
/*=========================================================

=========================================================*/
void UsuarioBean::accessLog(Map &map){
  BEAN(PhpUtil,util,"app.php.util")
  String
    fileName=getMap().get("php.action.log").asStr(),
    linha,
    exemplo=getStr("exemplo").trim(),
    row=fileLoad("admin/usuario/accessRow.html");
  File
    f;
  ByteArray
    buffer;
  TableDef
    def;
  Table
    table;

  table=def
    .add("data",C_DATETIME)
    .add("ip",C_TEXT,255)
    .add("url",C_TEXT,255)
    .add("action",C_TEXT,255)
    .create();

  f.open(fileName,1);
  f.readAll(buffer);
  buffer.seek(0);
  while(buffer.readLine(linha)){
    Split
      item=Split(linha,';');

    table
      .addNew()
      .putStr("data",item[0])
      .putStr("ip",item[1])
      .putStr("url",item[2])
      .putStr("action",item[3]);
  }
  if(exemplo.len())
    table.grep(&exemplo);
  buffer.reset();
  table.top();
  while(table.fetch())
    buffer.writef(
      row,
      util.format(table,"data").ptr(),
      util.format(table,"ip").ptr(),
      util.format(table,"url").ptr(),
      util.format(table,"action").ptr()
    );
  setStr("AccessTable","%s",buffer.toString().ptr());
  dispatch("admin/usuario/access.html");
}
/*=========================================================

=========================================================*/
void UsuarioBean::senhaTroca(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  BEAN(PhpUtil,util,"app.php.util")
  TableRef
    usuarioSenha=dat.use("usuarioSenha");
  Number
    id=util.userId();
  int
    x=usuarioSenha.find("id",&id);
  String
    erro,
    senha,
    senha0=getStr("senha0"),
    senha1=getStr("senha1"),
    senha2=getStr("senha2");

  if(!FOUND(x)){
    erro="A Senha não está definida. Não é você quem faz.";
    goto ERRO;
  }
  senha=usuarioSenha.go(x).getStr("senha");
  if(!senha.equals(senha0)){
    erro="A Senha atual não é essa.";
    goto ERRO;
  }
  if(!senha1.equals(senha2)){
    erro="As senhas nova/confirma não são iguais. Verifique!";
    goto ERRO;
  }
  usuarioSenha
    .putStr("senha",senha1)
    .save();
  goto FIM;
  ERRO:
  message(MSG_ERROR,erro);
  dispatch("admin/usuario/senhaTroca.html");
  return;
  FIM:
  message(MSG_SUCCESS,"Sua senha foi alterada.");
  dispatch("aplicacao.html");
}
