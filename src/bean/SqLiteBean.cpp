/*=========================================================

=========================================================*/
SqLiteBean::SqLiteBean(){
}
/*=========================================================

=========================================================*/
SqLiteBean::~SqLiteBean(){
}
/*=========================================================

=========================================================*/
void SqLiteBean::datList(Map &map){
  Split
    pasta=Split(getMap().get("dat").asStr(),','),
    arquivo;

  for(int i=0;i<pasta.len();i++){
    String
      folder=pasta[i];
    Dir
      dir;

    dir.open(getMap().get(folder).asStr());
    for(int i=0;i<dir.len();i++){
      DirItem
        &item=dir.item(i);

      if(item.type==C_FILE&&FOUND(item.name.inStr(".dat")))
        arquivo.add(String().sprintf(
          "%s://%s",
          folder.ptr(),
          item.name.ptr()
        ));
    }
  }
  printf("%s",arquivo.sort().join(',').ptr());
}
/*=========================================================

=========================================================*/
void SqLiteBean::datImport(Map &map){
  char
    create=getInt("create"),
    drop=getInt("drop");
  String
    database=getStr("database").trim(),
    erro;
  Split
    selecionado=Split(getStr("selecionado"),'|');
  PhpDat
    dat;
  SqLite
    sql;
  ByteArray
    buffer;
  C_CALL
    call;
  void
    **p=(void**)&call,
    (Object::*to)(C_CALL)=0;

  inject(&dat);
  *(void**)&to=METHOD(&SqLite::toSql);
  p[0]=(void*)(uint)drop;
  p[1]=(void*)(uint)create;
  p[2]=&buffer;

  for(int i=0;i<selecionado.len();i++){
    Split
      split=Split(selecionado[i],"://");
    TableRef
      table=dat.use(split[1].replace(".dat",""));

    p[i+3+0]=&table;
    p[i+3+1]=0;
  }
  (sql.*to)(call);

  sql.init(getStr("sqlite.dll"));
  sql.open(database);
  sql.shell(
    buffer,
    erro,
    getStr("sqlite.ld"),
    getStr("sqlite.lib"),
    getStr("sqlite.cmd")
  );
  if(erro.len()){
    String
      name=getStr("sqlite.erro");

    if(name.len()){
      File
        f;

      f.open(name);
      f.truncate();
      f.write(buffer);
      f.write(erro);
    }
  }
  if(!erro.len())
    erro="Realizado com Sucesso.";
  printf("%s",erro.ptr());
}
/*=========================================================

=========================================================*/
void SqLiteBean::listTable(Split &list){
  Split
    item=Split(getMap().get("dat").asStr(),',');

  list.reset();
  for(int i=0;i<item.len();i++){
    String
      pasta=getMap().get(item[i]).asStr();
    Dir
      dir;

    dir.open(pasta);
    for(int i=0;i<dir.len();i++){
      DirItem
        &item=dir.item(i);

      if(item.type==C_FILE&&FOUND(item.name.inStr(".dat")))
        list.add(item.name.copy().replace(".dat",""));
    }
  }
  list.sort();
}
/*=========================================================

=========================================================*/
int cSqliteConnectDat(void *db,void *aux,int argc,char **argv,void **vtab,char **erro){
  Map
    argm;
  String
    name;
  Table
    *table=Table().instance();
  SQLITE_VTAB
    *tab;
  SQLITE_AUX
    *paux=(SQLITE_AUX*)aux;
  ByteArray
    buffer;
  PhpDat
    *dat=(PhpDat*)paux->aux0;

  argm.loadArg(argc,argv);
  name=argm.get("dat").asStr();
  table->operator=(dat->use(name));
  paux->sqLite->toSql(0,1,buffer,table->copy().removeAll().me(),0);
  cSqliteApi.sqlite3_declare_vtab(
    db,
    buffer.toString()
  );
  tab=(SQLITE_VTAB*)cMemAlloc(sizeof(SQLITE_VTAB));
  tab->file=cMemAddChar(name);
  tab->table=table;

  *vtab=tab;
  return 0;
}
/*=========================================================

=========================================================*/
int cSqliteCreateDat(void *db,void *aux,int argc,char **argv,void **vtab,char **erro){
  return cSqliteConnectDat(db,aux,argc,argv,vtab,erro);
}
/*=========================================================

=========================================================*/
void SqLiteBean::moduleDat(SqLite &sql){
  BEANMAP(PhpDat,dat,"app.php.dat")
  SQLITE_MODULE
    *module=(SQLITE_MODULE*)cMemAlloc(sizeof(SQLITE_MODULE));
  SQLITE_AUX
    *aux=(SQLITE_AUX*)cMemAlloc(sizeof(SQLITE_AUX));
  Split
    list;
  ByteArray
    buffer;
  SqLiteResult
    res;
  char
    ok;

  cSqliteModuleInit(*module);
  module->xCreate=cSqliteCreateDat;
  module->xConnect=cSqliteCreateDat;
  aux->aux0=&dat;
  sql.createModule("dat",*module,*aux);
  listTable(list);
  for(int i=0;i<list.len();i++)
    buffer.writef(
      "create virtual table temp.%s using dat(dat=%s);\n",
      list[i].ptr(),
      list[i].ptr()
    );
  ok=sql.query(buffer.toString(),res);
  if(!ok)
    printf("erro:%s\n",sql.getError().ptr());
  gc.add(module);
  gc.add(aux);
}
/*=========================================================

=========================================================*/
void SqLiteBean::datQuery(Map &map){
  String
    database=getStr("database").trim(),
    command=getStr("command").trim();
  SqLite
    sql;
  SqLiteResult
    result;
  char
    useDat=getInt("useDat");

  sql.init(getStr("sqlite.dll"));
  sql.open(database);
  if(useDat)
    moduleDat(sql);
  if(!sql.query(command.copy(),result))
    printf("erro:%s\n",sql.getError().ptr());
  else
    for(int i=0;i<result.len();i++){
      SqLiteItem
        &item=result.get(i);

      switch(item.getType()){
        case SQLITE_RES_NUMBER:
          printf("affected:%d\n",item.getNumber().asInt());
          break;
        case SQLITE_RES_TABLE:
          printf("table:%s\n",
            Base64().encode(
            TableHtml().toHtml(
            item.getTable())
            .ref())
            .ptr()
          );
          break;
      }
    }
}
/*=========================================================

=========================================================*/
void SqLiteBean::tables(Map &map){
  String
    database=getStr("database"),
    erro;
  SqLite
    sql;
  ByteArray
    buffer;
  char
    ok;

  buffer.writeStr(".schema");
  sql.init(getStr("sqlite.dll"));
  ok=sql.open(database);
  if(!ok){
    printf("ERRO: not open.");
    return;
  }
  sql.shell(
    buffer,
    erro,
    getStr("sqlite.ld"),
    getStr("sqlite.lib"),
    getStr("sqlite.cmd")
  );
  {
    String
      linha;

    erro=erro
      .replace("CREATE TABLE"," ")
      .replace("FIM SQLITE SHELL"," ")
      .replace("  "," ")
      .replace("; ",";")
      .replace("\n\n","\n")
      .replace(" \n","\n")
      .replace("\n ","\n")
      .trim();
    buffer.reset().writeStr(erro).seek(0);
    while(buffer.readLine(linha))
      printf("%s\n",linha.ptr());
  }
}
/*=========================================================

=========================================================*/
void SqLiteBean::catalogo(Map &map){
  Split
    item,files;
  ByteArray
    res;
  PhpDat
    dat;
  class Ordem:public Object{
    public:
    char compare(Sort &sort,void *a,void *b){
      String
        sa=*(char**)a,
        sb=*(char**)b;
      int
        pa=sa.inStrRev('/'),
        pb=sb.inStrRev('/');

      return sa.mid(pa+1).compare(sb.mid(pb+1));
    }
  };
  Ordem
    ordem;

  inject(&dat);
  listTable(files);
  for(int i=0;i<files.len();i++){
    TableRef
      table=dat.use(files[i]);

    res.writef(
      "<div><a>%s</a>"
      "<table><tr><th>Field</th><th>Type</th><th>Size</th></tr>",
      files[i].ptr()
    );
    for(int i=0;i<table.getDef().len();i++)
      res.writef(
        "<tr><td>%s</td><td>%s</td><td>%d</td></tr>",
        table.getDef().get(i).getName().ptr(),
        Type().name(table.getDef().get(i).getType()).ptr(),
        table.getDef().get(i).getSize()
      );

    res.writeStr("</table></div>");
  }
  printf("%s",res.toString().ptr());
}
/*=========================================================

=========================================================*/
void SqLiteBean::listDat(Split &list){
  listTable(list);
}
