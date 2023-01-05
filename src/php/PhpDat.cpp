/*=========================================================

=========================================================*/
PhpDat::PhpDat(){
}
PhpDat::~PhpDat(){
}
/*=========================================================

=========================================================*/
char PhpDat::open(cchar *name,Table &table){
  Split
    item=Split(getMap().get("dat").asStr(),',');

  table.reset();
  for(int i=0;i<item.len();i++){
    String
      base=getMap().get(item[i]).asStr().append('/').append(name),
      testDat,
      testDef;
    File
      f;

    testDat=base.copy().append(".dat");
    if(f.access(testDat,"rw-")){
      int
        erro;

      f.open(testDat,1);
      table.open(f,&erro);
      if(erro)
        return 0;
      return 1;
    }
    testDef=base.copy().append(".def");
    if(f.access(testDef,"r--")){
      String
        buffer;
      TableDef
        def;
      int
        erro=0;

      f.open(testDef,1);
      f.readAll(buffer);
      table=def
        .fromDef(buffer)
        .create()
        .setName(name)
        .setFileName(testDat)
        .save(&erro);
      if(erro)
        return 0;
      return 1;
    }
  }
  return 0;
}
/*=========================================================

=========================================================*/
void PhpDat::close(cchar *name){
  MapItem
    *item=&tables.get(name);

  if(item->isNull())
    return;
  else{
    void
      *ptr=item->asPtr();

    gcTables.remove(ptr);
    tables.remove(name);
  }
}
/*=========================================================

=========================================================*/
Table &PhpDat::use(cchar *name){
  MapItem
    &item=tables.get(name);

  if(!item.isNull())
    return *(Table*)item.asObj();
  else{
    Table
      &table=*Table().instance();

    if(open(name,table)){
      tables.setValue(name,&table,1);
      return table;
    }
    gcTables.add(&table);
    return table;
  }
}
/*=========================================================

=========================================================*/
String PhpDat::serialize(Table &table){
  SSL32
    *ssl;
  ByteArray
    buffer;

  getMap().get("app.ssl32").pObj(&ssl);
  table.toStream(buffer);
  ssl->encrypt(0,buffer);
  return buffer.toString();
}
/*=========================================================

=========================================================*/
Table PhpDat::unserialize(cchar *value){
  SSL32
    *ssl;
  Table
    table;
  ByteArray
    buffer;
  int
    erro;

  getMap().get("app.ssl32").pObj(&ssl);
  buffer.writeStr(value);
  ssl->decrypt(0,buffer);
  table.fromStream(buffer,&erro);
  table.go(0);
  if(erro)
    table.reset();
  return table;
}
/*=========================================================

=========================================================*/
void PhpDat::listTables(Split &list){
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
