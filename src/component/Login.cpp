/*=========================================================

=========================================================*/
Login::Login(){
}
Login::~Login(){
}
String Login::getMessage(void){return msg;}
String Login::getNome(void){return nome;}
String Login::getSerial(void){return serial;}
Number Login::getUsuarioId(void){return usuarioId;}
Split &Login::getGrupo(void){return grupo;}
Split &Login::getPapel(void){return papel;}
/*=========================================================

=========================================================*/
char Login::login(const String &user,const String pass){
  TableRef
    usuario=findUser(user);
  PermissaoList
    permissao;

  if(!usuario.len()){
    msg="Não sei quem você é.";
    return 0;
  }
  if(!password(usuario,pass)){
    block(usuario,0);
    msg=String().sprintf(
      "Você não pode entrar. %s",
      msg.len()?msg.ptr():""
    );
    return 0;
  }
  if(blocked(usuario)){
    block(usuario,0);
    msg="Você não pode entrar [bloqueado].";
    return 0;
  }
  if(!actived(usuario)){
    block(usuario,0);
    msg="Você não pode entrar [ativo].";
    return 0;
  }
  block(usuario,1);
  usuarioId=usuario.getInt("id");
  inject(&permissao);
  nome=usuario.getStr("nome");
  permissao.gruposDoUsuario(usuarioId.asInt(),grupo);
  permissao.papelsDoGrupo(grupo,papel);
  serialize(usuario);

  return 1;
}
/*=========================================================

=========================================================*/
void Login::serialize(TableRef usuario){
  BEANMAP(SSL32,ssl,"app.ssl32")
  ByteArray
    buffer;

  buffer.writef(
    "%d;%s;%s",
    usuario.getInt("id"),
    Date().now().format().ptr(),
    usuario.getStr("nome").ptr()
  );
  ssl.encrypt(0,buffer);
  serial=buffer.toString();
}
/*=========================================================

=========================================================*/
char Login::actived(TableRef usuario){
  return usuario.getInt("ativo");
}
/*=========================================================

=========================================================*/
char Login::blocked(TableRef usuario){
  return usuario.getInt("bloqueado");
}
/*=========================================================

=========================================================*/
void Login::block(TableRef usuario,char ok){
  BEANMAP(PhpDat,dat,"app.php.dat")
  TableRef
    usuarioLogin=dat.use("usuarioLogin");

  usuarioLogin
    .addNew()
    .putInt("usuario",usuario.getInt("id"))
    .putDte("data",Date().now())
    .putInt("ok",ok)
    .save();

  if(ok)
    return;
  else{
    int
      n=0,
      max=3;

    for(int i=usuarioLogin.len()-1;i>=0;i--){
      usuarioLogin.go(i);
      if(usuarioLogin.getInt("usuario")==usuario.getInt("id")){
        if(usuarioLogin.getInt("ok"))
          return;
        if((n+=1)>max){
          usuario
            .putInt("bloqueado",1)
            .save();
          return;
        }
      }
    }
  }
}
/*=========================================================

=========================================================*/
char Login::password(TableRef usuario,const String &pass){
  BEANMAP(PhpDat,dat,"app.php.dat")
  TableRef
    usuarioSenha=dat.use("usuarioSenha");
  int
    x=usuarioSenha.find("id",Number(usuario.getInt("id")).me());

  return !FOUND(x)
    ?0
    :usuarioSenha.go(x).getStr("senha").equals(pass);
}
/*=========================================================

=========================================================*/
TableRef Login::findUser(const String &user){
  BEANMAP(PhpDat,dat,"app.php.dat")
  TableRef
    usuario=dat.use("usuario");
  int
    x;
  Number
    id;

  if(FOUND(x=usuario.sort("matricula").find("matricula",&user))){
    id=usuario.go(x).getInt("id");
    return usuario.go(usuario.sort("id").find("id",&id));
  }
  if(FOUND(x=usuario.sort("cpf").find("cpf",&user))){
    id=usuario.go(x).getInt("id");
    return usuario.go(usuario.sort("id").find("id",&id));
  }
  usuario.sort("id");
  id=Number().parse(PSTRING(user));
  if(FOUND(x=usuario.find("id",&id)))
    return usuario.go(x);
  {
    TableRef
      table=*Table().instance();

    getPhp().getGcObject().add(&table);
    return table;
  }
}
/*=========================================================

=========================================================*/
String Login::grupoStr(void){
  return String('|')
    .append(grupo.join('|'))
    .append('|');
}
String Login::papelStr(void){
  return String('|')
    .append(papel.join('|'))
    .append('|');
}
/*=========================================================

=========================================================*/
char Login::fromCookie(void){
  String
    value=getStr("sessionUser").trim();

  if(!value.len())
    return 0;
  else{
    BEANMAP(SSL32,ssl,"app.ssl32")
    ByteArray
      buffer=value.ptr();

    if(!ssl.decrypt(0,buffer))
      return 0;
    else{
      Split
        item=Split(buffer.toString(),';');

      if(item.len()!=3)
        return 0;
      else{
        Number
          id=Number().parse(item[0]);
        Date
          data=Date().parse(item[1]);
        String
          nome=item[2];

        if(!id||!DateFormat().isDate(data)||!nome.len())
          return 0;
        else{
          TableRef
            usuario=findUser(id.toString());

          if(!usuario.len())
            return 0;
          else{
            PermissaoList
              permissao;

            inject(&permissao);
            usuarioId=usuario.getInt("id");
            nome=usuario.getStr("nome");
            permissao.gruposDoUsuario(usuarioId.asInt(),grupo);
            permissao.papelsDoGrupo(grupo,papel);
            setStr("app.user.nome",nome);
            setStr("app.user.grupo",grupoStr());
            setStr("app.user.id","%d",usuarioId.asInt());

            return 1;
          }
        }
      }
    }
  }
  return 1;
}


