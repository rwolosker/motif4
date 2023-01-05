GrupoPapel::GrupoPapel(){}
GrupoPapel::~GrupoPapel(){};
/*=========================================================

=========================================================*/
void GrupoPapel::papelList(Split &list){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    papel=dat.use("papel").copy().sort("nome").top();

  list.reset();
  papel.top();
  while(papel.fetch())
    list.add(papel.getStr("nome"));
}
/*=========================================================

=========================================================*/
void GrupoPapel::papeldoGrupo(int id,Split &list){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    papel=dat.use("papel").copy(),
    grupoPapel=dat.use("grupoPapel").copy().sort("grupo");
  int
    x=grupoPapel.find("grupo",Number(id).me());

  list.reset();
  if(FOUND(x)){
    grupoPapel.go(x-1);
    while(grupoPapel.fetch()&&grupoPapel.getInt("grupo")==id){
      int
        x=papel.find("id",Number(grupoPapel.getInt("papel")).me());

      if(FOUND(x))
        list.add(papel.go(x).getStr("nome"));
    }
    list.sort();
  }
}
/*=========================================================

=========================================================*/
String GrupoPapel::grupoNome(int id){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    grupo=dat.use("grupo").copy();
  int
    x=grupo.find("id",Number(id).me());

  return FOUND(x)
    ?grupo.go(x).getStr("nome")
    :String();
}
/*=========================================================

=========================================================*/
void GrupoPapel::save(int id,Split &list){
  BEANMAP(PhpDat,dat,"app.php.dat")
  TableRef
    grupoPapel=dat.use("grupoPapel");
  Table
    update=grupoPapel.copy(),
    papel=dat.use("papel").copy().sort("nome");

  grupoPapel.removeAll();
  update.top();
  while(update.fetch())
    if(update.getInt("grupo")!=id)
      grupoPapel.addReg(&update,0);
  for(int i=0;i<list.len();i++){
    int
      x=papel.find("nome",list[i].me());

    if(FOUND(x))
      grupoPapel
        .addNew()
        .putInt("grupo",id)
        .putInt("papel",papel.go(x).getInt("id"));
  }
  grupoPapel
    .sort("grupo")
    .save();
}
