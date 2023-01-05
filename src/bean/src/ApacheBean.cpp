ApacheBean::ApacheBean(){};
ApacheBean::~ApacheBean(){};
/*=========================================================

=========================================================*/
char ApacheBean::byDataDesc(TableSort &sort,void *a,void *b){
  int
    data=0,
    c;
  sort.colByName("data",&data);
  c=sort
    .getField(a,data).getDate().compare(
      sort.getField(b,data).getDate());
  return c*-1;
}
/*=========================================================

=========================================================*/
void ApacheBean::table(Map &map){
  String
    name=getStr("php.apache.log.dir"),
    row=fileLoad("admin/apache/row.html");
  Dir
    dir;
  File
    file;
  Table
    table;
  ByteArray
    buffer;

  table=TableDef()
    .add("data",C_DATETIME)
    .add("nome",C_TEXT,512)
    .add("tamanho",C_DOUBLE)
    .create();

  dir.open(name);
  for(int i=0;i<dir.len();i++){
    DirItem
      &item=dir.item(i);
    File
      f;

    if(item.type==C_DIR)
      continue;

    f.open(item.getAbsolutePath(),1);
    table
      .addNew()
      .putDte("data",f.modified())
      .putStr("nome",item.name)
      .putDbl("tamanho",f.size()/1024.);
  }
  table.sort(this,METHOD(&ApacheBean::byDataDesc)).top();
  while(table.fetch())
    buffer.writef(
      row,
      table.getDte("data").format().ptr(),
      table.getDbl("tamanho"),
      table.getStr("nome").ptr()
    );
  setStr("apacheLogTable","%s",buffer.toString().ptr());
  dispatch("admin/apache/log.html");
}
/*=========================================================

=========================================================*/
void ApacheBean::select(Map &map){
  String
    name=getStr("php.apache.log.dir"),
    arquivo=getStr("arquivo");
  File
    file;
  ByteArray
    buffer;

  file.open(name.append("/").append(arquivo),1);
  file.readAll(buffer);
  eval("header('Content-Type: text/plain');");
  printf("%s",buffer.toString().ptr());
}
