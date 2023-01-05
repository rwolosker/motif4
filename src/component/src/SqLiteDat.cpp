/*=========================================================

=========================================================*/
SqLiteDat::SqLiteDat(){

}
SqLiteDat::~SqLiteDat(){

}
/*=========================================================

=========================================================*/
SqLiteDat &SqLiteDat::setFileName(const String &fileName){this->fileName=fileName;return *this;}
String SqLiteDat::getFileName(void){return fileName;}
/*=========================================================

=========================================================*/
SqLite &SqLiteDat::getSqLite(void){
  return sql;
}
/*=========================================================

=========================================================*/
void SqLiteDat::configure(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  SQLITE_MODULE
    *module=(SQLITE_MODULE*)cMemAlloc(sizeof(SQLITE_MODULE));
  SQLITE_AUX
    *aux=(SQLITE_AUX*)cMemAlloc(sizeof(SQLITE_AUX));
  SqLiteBean
    sqlBean;
  Split
    list;
  ByteArray
    buffer;
  char
    ok;
  SqLiteResult
    res;

  inject(&sqlBean);
  gc.add(module);
  gc.add(aux);
  cSqliteModuleInit(*module);
  module->xCreate=cSqliteCreateDat;
  module->xConnect=cSqliteCreateDat;
  aux->aux0=&dat;
  sql.init(getStr("sqlite.dll"));
  sql.open(
    fileName.len()
      ?fileName
      :String("../data.db")
  );
  sql.createModule("dat",*module,*aux);
  sqlBean.listDat(list);
  for(int i=0;i<list.len();i++)
    buffer.writef(
      "create virtual table temp.%s using dat(dat=%s);\n",
      list[i].ptr(),
      list[i].ptr()
    );
  ok=sql.query(buffer.toString(),res);
  if(!ok){
    cPrintf("erro:%s\n",sql.getError().ptr());
    exit(1);
  }
}
