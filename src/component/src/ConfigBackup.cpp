/*=========================================================

=========================================================*/
void ConfigBackup::restoreLog(const String &tag,int total,int update,int insert){
  if(!log.getDef().len())
    log=TableDef()
      .add("table",C_TEXT,128)
      .add("insert",C_INT)
      .add("update",C_INT)
      .add("total",C_INT)
      .create();

  log
    .addNew()
    .putStr("table",tag)
    .putInt("insert",insert)
    .putInt("update",update)
    .putInt("total",total);
}
/*=========================================================

=========================================================*/
void ConfigBackup::unique(Table &table){
  Split
    item;

  table.top();
  while(table.fetch())
    item.add(String().sprintf(
      "%010d:%010d",
      table.getInt(0),
      table.getInt(1)
    ));
  item.sort().unique();

  table.removeAll();

  for(int i=0;i<item.len();i++){
    Split
      c=Split(item[i],':');

    table
      .addNew()
      .putStr(0,c[0])
      .putStr(1,c[1]);
  }
}
/*=========================================================

=========================================================*/
void ConfigBackup::mergeUsuario(Table &usuario,ByteArray &buffer){
  String
    linha;
  int
    u=0,t=0;

  buffer.seek(0);
  while(buffer.len()&&buffer.readLine(linha)){
    Split
      item=Split(linha,';');
    int
      x=usuario.sort("nome").find("nome",item[0].me());

    t+=1;

    if(FOUND(x))
      usuario.go(x);
    else{
      int
        id=usuario.sort("id").last().getInt("id")+1;

      usuario.addNew();
      usuario.putInt("id",id);
      u+=1;
    }
    for(int i=1;i<usuario.getDef().len();i++)
      usuario.putStr(i,item[i-1]);
  }
  restoreLog("usuario",t,t-u,u);
}
/*=========================================================

=========================================================*/
void ConfigBackup::mergeGrupo(Table &grupo,ByteArray &buffer){
  String
    linha;
  int
   t=0,u=0;

  buffer.seek(0);
  while(buffer.len()&&buffer.readLine(linha)){
    Split
      item=Split(linha,';');
    int
      x=grupo.sort("nome").find("nome",item[0].me());

    t+=1;

    if(FOUND(x))
      grupo.go(x);
    else{
      int
        id=grupo.sort("id").last().getInt("id")+1;

      grupo.addNew();
      grupo.putInt("id",id);
      u+=1;
    }
    for(int i=1;i<grupo.getDef().len();i++)
      grupo.putStr(i,item[i-1]);
  }
  restoreLog("grupo",t,t-u,u);
}
/*=========================================================

=========================================================*/
void ConfigBackup::mergeAlias(Table &alias,ByteArray &buffer){
  String
    linha;
  int
    t=0,u=0;

  buffer.seek(0);
  while(buffer.len()&&buffer.readLine(linha)){
    Split
      item=Split(linha,';');
    int
      x=alias.sort("alias").find("alias",item[0].me());

    t+=1;

    if(FOUND(x))
      alias.go(x);
    else{
      int
        id=alias.sort("id").last().getInt("id")+1;

      alias.addNew();
      alias.putInt("id",id);
      u+=1;
    }
    for(int i=1;i<alias.getDef().len();i++)
      alias.putStr(i,item[i-1]);
  }
  restoreLog("alias",t,t-u,u);
}
/*=========================================================

=========================================================*/
void ConfigBackup::mergePapel(Table &papel,ByteArray &buffer){
  String
    linha;
  int
    t=0,u=0;

  buffer.seek(0);
  while(buffer.len()&&buffer.readLine(linha)){
    Split
      item=Split(linha,';');
    int
      x=papel.sort("nome").find("nome",item[0].me());

    t+=1;

    if(FOUND(x))
      papel.go(x);
    else{
      int
        id=papel.sort("id").last().getInt("id")+1;

      papel.addNew();
      papel.putInt("id",id);
      u+=1;
    }
    for(int i=1;i<papel.getDef().len();i++)
      papel.putStr(i,item[i-1]);
  }
  restoreLog("papel",t,t-u,u);
}
/*=========================================================

=========================================================*/
void ConfigBackup::mergePublico(Table &publico,ByteArray &buffer){
  String
    linha;
  int
    t=0,u=0;

  buffer.seek(0);
  while(buffer.len()&&buffer.readLine(linha)){
    Split
      item=Split(linha,';');
    int
      x=publico.sort("nome").find("nome",item[0].me());

    t+=1;

    if(FOUND(x))
      publico.go(x);
    else{
      int
        id=publico.sort("id").last().getInt("id")+1;

      publico.addNew();
      publico.putInt("id",id);
      u+=1;
    }
    for(int i=1;i<publico.getDef().len();i++)
      publico.putStr(i,item[i-1]);
  }
  restoreLog("publico",t,t-u,u);
}
/*=========================================================

=========================================================*/
void ConfigBackup::mergeGrupoPapel(Table &grupo,Table &papel,Table &grupoPapel,ByteArray &buffer){
  String
    linha;
  class Ordena:public Object{
    public:
    //---------------------------------------------------
    char byGrupoPapel(TableSort &sort,void *a,void *b){
      int
        ga=sort.getField(a,"grupo").getInt(),
        gb=sort.getField(b,"grupo").getInt(),
        pa=sort.getField(a,"papel").getInt(),
        pb=sort.getField(b,"papel").getInt(),
        g=ga==gb?0:ga<gb?-1:1,
        p=pa==pb?0:pa<pb?-1:1;

      return g?g:p;
    }
    //---------------------------------------------------
    char findGrupoPapel(TableSort &sort,void *a){
      int
        ga=sort.getField(a,"grupo").getInt(),
        pa=sort.getField(a,"papel").getInt(),
        gq=sort.getParamNumber(0),
        pq=sort.getParamNumber(1),
        g=gq==ga?0:gq<ga?-1:1,
        p=pq==pa?0:pq<pa?-1:1;

      return g?g:p;
    }

  };
  Ordena
    ordena;
  int
    t=0,u=0;

  grupo.sort("nome");
  papel.sort("nome");
  grupoPapel.sort(
    &ordena,
    METHOD(&Ordena::byGrupoPapel)
  );

  buffer.seek(0);
  while(buffer.len()&&buffer.readLine(linha)){
    Split
      item=Split(linha,';');
    int
      xg=grupo.find("nome",item[0].me()),
      xp=papel.find("nome",item[1].me()),
      xgp=grupoPapel.findEx(
        ordena,
        METHOD(&Ordena::findGrupoPapel),
        Number(grupo.go(xg).getInt("id")).me(),
        Number(papel.go(xp).getInt("id")).me()
      );

    t+=1;
    if(FOUND(xg)&&FOUND(xp)&&!FOUND(xgp)&&(u+=1))
      grupoPapel
        .addNew()
        .putInt(0,grupo.go(xg).getInt("id"))
        .putInt(1,papel.go(xp).getInt("id"))
        .sort(
          &ordena,
          METHOD(&Ordena::byGrupoPapel));

  }
  restoreLog("grupoPapel",t,0,u);
}
/*=========================================================

=========================================================*/
void ConfigBackup::mergeUsuarioGrupo(Table &usuario,Table &grupo,Table &usuarioGrupo,ByteArray &buffer){
  String
    linha;
  class Ordena:public Object{
    public:
    //---------------------------------------------------
    char byUsuarioGrupo(TableSort &sort,void *a,void *b){
      int
        ua=sort.getField(a,"usuario").getInt(),
        ub=sort.getField(b,"usuario").getInt(),
        ga=sort.getField(a,"grupo").getInt(),
        gb=sort.getField(b,"grupo").getInt(),
        u=ua==ub?0:ua<ub?-1:1,
        g=ga==gb?0:ga<gb?-1:1;

      return u?u:g;
    }
    //---------------------------------------------------
    char findUsuarioGrupo(TableSort &sort,void *a){
      int
        ua=sort.getField(a,"usuario").getInt(),
        ga=sort.getField(a,"grupo").getInt(),
        uq=sort.getParamNumber(0),
        gq=sort.getParamNumber(1),
        u=uq==ua?0:uq<ua?-1:1,
        g=gq==ga?0:gq<ga?-1:1;

      return u?u:g;
    }

  };
  Ordena
    ordena;
  int
    t=0,u=0;


  grupo.sort("nome");
  usuario.sort("nome");
  usuarioGrupo.sort(
    &ordena,
    METHOD(&Ordena::byUsuarioGrupo)
  );


  buffer.seek(0);
  while(buffer.len()&&buffer.readLine(linha)){
    Split
      item=Split(linha,';');
    int
      xu=usuario.find("nome",item[0].me()),
      xg=grupo.find("nome",item[1].me()),
      xug=usuarioGrupo.findEx(
        ordena,
        METHOD(&Ordena::findUsuarioGrupo),
        Number(usuario.go(xu).getInt("id")).me(),
        Number(grupo.go(xg).getInt("id")).me()
      );

    t+=1;
    if(FOUND(xg)&&FOUND(xu)&&!FOUND(xug)&&(u+=1))
      usuarioGrupo
        .addNew()
        .putInt(0,usuario.go(xu).getInt("id"))
        .putInt(1,grupo.go(xg).getInt("id"))
        .sort(
          &ordena,
          METHOD(&Ordena::byUsuarioGrupo)
        );
  }
  restoreLog("grupoUsuario",t,0,u);
}
/*=========================================================

=========================================================*/
ConfigBackup::ConfigBackup(){}
ConfigBackup::~ConfigBackup(){}
/*=========================================================

=========================================================*/
String ConfigBackup::backup(void){
  BEANMAP(PhpDat,dat,"app.php.dat")
  BEAN(PhpUtil,util,"app.php.util")
  ByteArray
    buffer;
  Table
    usuario=dat.use("usuario").copy(),
    grupo=dat.use("grupo").copy().sort("nome"),
    papel=dat.use("papel").copy().sort("nome"),
    publico=dat.use("publico").copy().sort("nome"),
    alias=dat.use("alias").copy().sort("alias"),
    grupoPapel=dat.use("grupoPapel").copy(),
    usuarioGrupo=dat.use("usuarioGrupo").copy();

  usuario.top();
  while(usuario.fetch())
    buffer.writef(
      "usuario:%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
      util.format(usuario,"nome").ptr(),
      util.format(usuario,"matricula").ptr(),
      util.format(usuario,"cpf").ptr(),
      util.format(usuario,"telefone").ptr(),
      util.format(usuario,"email").ptr(),
      util.format(usuario,"ativo").ptr(),
      util.format(usuario,"bloqueado").ptr(),
      util.format(usuario,"uid").ptr(),
      util.format(usuario,"ldap").ptr(),
      util.format(usuario,"cerberus").ptr()
    );
  grupo.top();
  while(grupo.fetch())
    buffer.writef(
      "grupo:%s\n",
      util.format(grupo,"nome").ptr()
    );
  alias.top();
  while(alias.fetch())
    buffer.writef(
      "alias:%s;%s;%s\n",
      util.format(alias,"alias").ptr(),
      util.format(alias,"real").ptr(),
      util.format(alias,"ativo").ptr()
    );
  papel.top();
  while(papel.fetch())
    buffer.writef(
      "papel:%s\n",
      util.format(papel,"nome").ptr()
    );
  publico.top();
  while(publico.fetch())
    buffer.writef(
      "publico:%s;%s\n",
      util.format(publico,"nome").ptr(),
      util.format(publico,"ativo").ptr()
    );
  grupoPapel.top();
  grupo.sort("id");
  papel.sort("id");
  usuario.sort("id");
  while(grupoPapel.fetch()){
    Number
      idGrupo=grupoPapel.getInt("grupo"),
      idPapel=grupoPapel.getInt("papel");
    int
      xGrupo=grupo.find("id",&idGrupo),
      xPapel=papel.find("id",&idPapel);

    if(FOUND(xGrupo)&&FOUND(xPapel))
      buffer.writef(
        "grupoPapel:%s;%s\n",
        util.format(grupo.go(xGrupo),"nome").ptr(),
        util.format(papel.go(xPapel),"nome").ptr()
      );
  }
  usuarioGrupo.top();
  while(usuarioGrupo.fetch()){
    Number
      idUsuario=usuarioGrupo.getInt("usuario"),
      idGrupo=usuarioGrupo.getInt("grupo");
    int
      xUsuario=usuario.find("id",&idUsuario),
      xGrupo=grupo.find("id",&idGrupo);

    if(FOUND(xGrupo)&&FOUND(xUsuario))
      buffer.writef(
        "usuarioGrupo:%s;%s\n",
        util.format(usuario.go(xUsuario),"nome").ptr(),
        util.format(grupo.go(xGrupo),"nome").ptr()
      );
  }

  return Split(buffer.toString(),'\n').sort().join('\n');
}
/*=========================================================

=========================================================*/
String ConfigBackup::restore(const String &path,ByteArray &buffer){
  String
    linha;
  Map
    keys;
  PhpDat
    dat;
  Table
    usuario,
    grupo,
    papel,
    alias,
    publico,
    grupoPapel,
    usuarioGrupo;
  ByteArray
    *txt=0;
  Split
    opcoes=Split(getStr("opcao"),',').sort();

  inject(&dat);
  usuario=dat.use("usuario");
  grupo=dat.use("grupo");
  papel=dat.use("papel");
  alias=dat.use("alias");
  publico=dat.use("publico");
  grupoPapel=dat.use("grupoPapel");
  usuarioGrupo=dat.use("usuarioGrupo");

  log.reset();

  buffer.seek(0);
  while(buffer.readLine(linha)){
    int
      i=linha.inStr(':');

    if(!FOUND(i))
      continue;
    else{
      String
        key=linha.mid(0,i),
        value=linha.mid(i+1);
      MapItem
        &item=keys.get(key);
      ByteArray
        *buffer=0;

      if(!item.isNull())
        item.pObj(&buffer);
      else{
        buffer=ByteArray().instance();
        keys.setValue(key,buffer,1);
      }
      buffer->writeStr(value.append('\n'));
    }
  }
  #define IMPORT(tag,metodo,table)\
    keys.get(tag).pObj(&txt);\
    if(txt){\
      if(!opcoes.contains(tag))\
        txt->reset();\
      metodo(table,*txt);\
    }
  #define IMPORT3(tag,metodo,table0,table1,table2)\
    keys.get(tag).pObj(&txt);\
    if(txt){\
      if(!opcoes.contains(tag))\
        txt->reset();\
      metodo(table0,table1,table2,*txt);\
    }
  IMPORT ("usuario",mergeUsuario,usuario)
  IMPORT ("grupo",mergeGrupo,grupo)
  IMPORT ("alias",mergeAlias,alias)
  IMPORT ("publico",mergePublico,publico)
  IMPORT ("papel",mergePapel,papel)
  IMPORT3("grupoPapel",mergeGrupoPapel,grupo,papel,grupoPapel)
  IMPORT3("usuarioGrupo",mergeUsuarioGrupo,usuario,grupo,usuarioGrupo)

  unique(grupoPapel);
  unique(usuarioGrupo);

  usuario.sort("id").save();
  grupo.sort("id").save();
  papel.sort("id").save();
  publico.sort("nome").save();
  alias.sort("alias").save();
  grupoPapel.sort("grupo").save();
  usuarioGrupo.sort("usuario").save();

  #undef IMPORT
  #undef IMPORT3

  return TableHtml().toHtml(log.sort("tag"));
}
