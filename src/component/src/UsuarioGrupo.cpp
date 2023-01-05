UsuarioGrupo::UsuarioGrupo(){}
UsuarioGrupo::~UsuarioGrupo(){};
/*=========================================================

=========================================================*/
void UsuarioGrupo::grupoList(Split &list){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    grupo=dat.use("grupo").copy().sort("nome").top();

  list.reset();
  grupo.top();
  while(grupo.fetch())
    list.add(grupo.getStr("nome"));
}
/*=========================================================

=========================================================*/
void UsuarioGrupo::grupodoUsuario(int id,Split &list){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    grupo=dat.use("grupo").copy(),
    usuarioGrupo=dat.use("usuarioGrupo").copy().sort("usuario");
  int
    x=usuarioGrupo.find("usuario",Number(id).me());

  list.reset();
  if(FOUND(x)){
    usuarioGrupo.go(x-1);
    while(usuarioGrupo.fetch()&&usuarioGrupo.getInt("usuario")==id){
      int
        x=grupo.find("id",Number(usuarioGrupo.getInt("grupo")).me());

      if(FOUND(x))
        list.add(grupo.go(x).getStr("nome"));
    }
    list.sort();
  }
}
/*=========================================================

=========================================================*/
String UsuarioGrupo::usuarioNome(int id){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    usuario=dat.use("usuario").copy();
  int
    x=usuario.find("id",Number(id).me());

  return FOUND(x)
    ?usuario.go(x).getStr("nome")
    :String();
}
/*=========================================================

=========================================================*/
void UsuarioGrupo::save(int id,Split &list){
  BEANMAP(PhpDat,dat,"app.php.dat")
  TableRef
    usuarioGrupo=dat.use("usuarioGrupo");
  Table
    update=usuarioGrupo.copy(),
    grupo=dat.use("grupo").copy().sort("nome");

  usuarioGrupo.removeAll();
  update.top();
  while(update.fetch())
    if(update.getInt("usuario")!=id)
      usuarioGrupo.addReg(&update,0);
  for(int i=0;i<list.len();i++){
    int
      x=grupo.find("nome",list[i].me());

    if(FOUND(x))
      usuarioGrupo
        .addNew()
        .putInt("usuario",id)
        .putInt("grupo",grupo.go(x).getInt("id"));
  }
  usuarioGrupo
    .sort("usuario")
    .save();
}
