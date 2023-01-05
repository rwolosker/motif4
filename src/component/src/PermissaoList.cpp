PermissaoList::PermissaoList(){};
PermissaoList::~PermissaoList(){};
/*=========================================================

=========================================================*/
char PermissaoList::byUsuarioGrupoPapel(Sort &sort,void *a,void *b){
  TableSort
    &ts=(TableSort&)sort;
  char
    c;
  static int
    usuarioNome=0,
    grupoNome=0,
    papelNome=0;
  Field
    *fa,*fb;

  if(!usuarioNome)
    usuarioNome=ts.getTable().getDef().get("usuario.nome").getCol();
  if(!grupoNome)
    grupoNome=ts.getTable().getDef().get("grupo.nome").getCol();
  if(!papelNome)
    papelNome=ts.getTable().getDef().get("papel.nome").getCol();

  fa=&ts.getField(a,usuarioNome);
  fb=&ts.getField(b,usuarioNome);
  if((c=fa->getString().compare(fb->getString())))
    return c;
  fa=&ts.getField(a,grupoNome);
  fb=&ts.getField(b,grupoNome);
  if((c=fa->getString().compare(fb->getString())))
    return c;
  fa=&ts.getField(a,papelNome);
  fb=&ts.getField(b,papelNome);
  return fa->getString().compare(fb->getString());
}
/*=========================================================

=========================================================*/
void PermissaoList::list(Table &table){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    usuario=dat.use("usuario").copy(),
    grupo=dat.use("grupo").copy(),
    papel=dat.use("papel").copy(),
    usuarioGrupo=dat.use("usuarioGrupo").copy(),
    grupoPapel=dat.use("grupoPapel").copy();

  table=TableJoin().join(&usuario,&grupo,&papel,0);
  usuarioGrupo.sort("usuario").top();
  grupoPapel.sort("grupo").top();
  usuario.top();

  while(usuario.fetch()){
    Number
      idUsuario=usuario.getInt("id"),
      idGrupo,
      idPapel;
    int
      x=usuarioGrupo.find("usuario",&idUsuario);

    if(!FOUND(x))
      continue;
    usuarioGrupo.go(x-1);
    while(usuarioGrupo.fetch()&&idUsuario==usuarioGrupo.getInt("usuario")){
      idGrupo=usuarioGrupo.getInt("grupo");
      x=grupoPapel.find("grupo",&idGrupo);

      if(!FOUND(x))
        continue;
      grupo.go(grupo.find("id",&idGrupo));
      grupoPapel.go(x-1);
      while(grupoPapel.fetch()&&idGrupo==grupoPapel.getInt("grupo")){
        idPapel=grupoPapel.getInt("papel");
        papel.go(papel.find("id",&idPapel));

        table.addReg(&usuario,&grupo,&papel,0);
      }
    }
  }
  table.sort(this,METHOD(&PermissaoList::byUsuarioGrupoPapel));
}
/*=========================================================

=========================================================*/
PermissaoList &PermissaoList::gruposDoUsuario(int id,Split &list){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    grupo=dat.use("grupo").copy(),
    usuarioGrupo=dat.use("usuarioGrupo").copy().top();
  static int
    USUARIO=0,
    GRUPO=0;
  int
    x;

  if(!USUARIO)
    USUARIO=usuarioGrupo.getDef().get("usuario").getCol();
  if(!GRUPO)
    GRUPO=usuarioGrupo.getDef().get("grupo").getCol();
  x=usuarioGrupo.find(USUARIO,Number(id).me());
  list.reset();
  if(FOUND(x)){
    usuarioGrupo.go(x-1);
    while(usuarioGrupo.fetch()&&usuarioGrupo.getInt(USUARIO)==id){
      x=grupo.find(0,Number(usuarioGrupo.getInt(GRUPO)).me());
      list.add(grupo.go(x).getStr(1));
    }
  }
  list
    .sort()
    .unique();
  return *this;
}
/*=========================================================

=========================================================*/
PermissaoList &PermissaoList::papelsDoGrupo(Split &grupoList,Split &list){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    grupo=dat.use("grupo").copy().sort("nome"),
    papel=dat.use("papel").copy(),
    grupoPapel=dat.use("grupoPapel").copy();
  static int
    PAPEL=0,
    GRUPO=0;
  int
    x;

  list.reset();
  if(!PAPEL)
    PAPEL=grupoPapel.getDef().get("papel").getCol();
  if(!GRUPO)
    GRUPO=grupoPapel.getDef().get("grupo").getCol();
  for(int i=0;i<grupoList.len();i++){
    Number
      id=grupo.go(grupo.find(1,grupoList[i].me())).getInt(0);

    x=grupoPapel.find(GRUPO,&id);
    if(FOUND(x)){
      grupoPapel.go(x-1);
      while(grupoPapel.fetch()&&id==grupoPapel.getInt(GRUPO)){
        papel.go(papel.find("id",Number(grupoPapel.getInt(PAPEL)).me()));
        list.add(papel.getStr("nome"));
      }
    }
  }
  list
    .sort()
    .unique();
  return *this;
}
