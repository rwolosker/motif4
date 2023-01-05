LoginBean::LoginBean(){}
LoginBean::~LoginBean(){}
/*=========================================================

=========================================================*/
void LoginBean::login(Map &map){
  String
    usuario=getStr("usuario"),
    senha=getStr("senha");
  Login
    &login=*(Login*)getFactory().getBean("app.login",getStr("php.login.class"));
  char
    ok=login.login(usuario,senha);

  if(ok){
    setStr("app.user.grupo","%s",login.grupoStr().ptr());
    setStr("app.user.nome","%s",login.getNome().ptr());
    eval(
      "setcookie('sessionUser','%s',time()+60*60*24*30,'/');",
      login.getSerial().ptr()
    );
  }

  if(ok)
    dispatch("aplicacao.html");
  else{
    message(MSG_ERROR,"%s",login.getMessage().ptr());
    dispatch("admin/usuario/login.html");
  }
}
/*=========================================================

=========================================================*/
void LoginBean::list(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  String
    exemplo=getStr("exemplo").trim(),
    row=fileLoad("admin/login/listTableRow.html");
  Table
    usuarioLogin=dat.use("usuarioLogin"),
    usuario=dat.use("usuario"),
    list=TableJoin().join(&usuario,&usuarioLogin,0);
  ByteArray
    buffer;
  PhpUtil
    util;

  usuarioLogin.top();
  while(usuarioLogin.fetch()){
    Number
      id=usuarioLogin.getInt("usuario");
    int
      x=usuario.find("id",&id);

    if(FOUND(x))
      list.addReg(usuario.go(x).me(),&usuarioLogin,0);
  }
  if(exemplo.len())
    list.grep(&exemplo);

  list.top();
  while(list.fetch())
    buffer.writef(
      row,
      util.format(list,"usuarioLogin.data").ptr(),
      util.format(list,"usuarioLogin.ok").ptr(),
      util.format(list,"usuario.matricula").ptr(),
      util.format(list,"usuario.nome").ptr()
    );
  setStr("LoginList","%s",buffer.toString().ptr());
  dispatch("admin/login/list.html");
}
