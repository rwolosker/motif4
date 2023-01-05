/*=========================================================

=========================================================*/
PhpBind::PhpBind(){
  initPhpBind();
}
void PhpBind::initPhpBind(void){
  util=0;
}
/*=========================================================

=========================================================*/
PhpUtil &PhpBind::getUtil(void){
  if(util)
    return *util;
  else{
    getFactory().getBean("app.php.util","PhpUtil",&util);
    return *util;
  }
}
PhpBind::~PhpBind(){

}
/*=========================================================

=========================================================*/
void PhpBind::bindBoolean(MapItem &map,Field &field,int flag){
  field.setInt(map.isNull()?0:1);
}
/*=========================================================

=========================================================*/
void PhpBind::bindDate(MapItem &map,Field &field,int flag){
  if(map.isNull())
    field.setInt(0);
  else
    field.setString(
      getUtil().dteInput(map.asStr())
    );
}
/*=========================================================

=========================================================*/
String PhpBind::valueInteger(const String &value){
  String
    src=value,
    dst;
  char
    *pv=src.ptr(),
    *pu=dst.resize(1024).ptr();

  while(pv&&*pv)
    if(*pv>='0'&&*pv<='9')
      *pu++=*pv++;
    else
      pv++;

  return dst;
}
/*=========================================================

=========================================================*/
void PhpBind::bindNumber(MapItem &map,Field &field,int flag){
  if(map.isNull())
    field.setInt(0);
  else{
    String
      value=getUtil().numInput(map.asStr());

    if(flag&BFLOATBR)
      value=value
        .replace('.',"")
        .replace(',','.');
    if(flag==BINTEGER)
      value=valueInteger(value);
    field.setNumber(Number().parse(value));
  }
}
/*=========================================================

=========================================================*/
void PhpBind::bindText(MapItem &map,Field &field,int flag){
  if(map.isNull())
    field.setText(0);
  else{
    String
      value=map.asStr();

    if(flag&BINTEGER){
      value=valueInteger(value);
      goto NEXT;
    }
    if(flag&BNOINPUT)
      goto NEXT;
    if(flag&BTRIMONLY){
      value=value.trim();
      goto NEXT;
    }
    if(flag&BCAPITALIZE)
      value=getUtil().strInput(value,1);
    if((flag&BNOCAPITALIZE)||!flag)
      value=getUtil().strInput(value,0);

    NEXT:
      field.setString(value);
  }
}
/*=========================================================

=========================================================*/
PhpBind &PhpBind::bind(cchar *key,Table &table,cchar *field,int flag){
  MapItem
    &map=getMap().get(key);
  Field
    &f=table.getDef().get(field?field:key);

  switch(f.getType()){
    case C_INT:
    case C_DOUBLE:
      bindNumber(map,f,flag);
      break;
    case C_TEXT:
    case C_CHAR:
      bindText(map,f,flag);
      break;
    case C_BOOL:
      bindBoolean(map,f,flag);
      break;
    case C_DATE:
    case C_DATETIME:
      bindDate(map,f,flag);
      break;
  }
  return *this;
}
/*=========================================================

=========================================================*/
PhpBind &PhpBind::bind(cchar *key,Object *dst,int flag){
  Table
    table;
  String
    tipo=getFactory().getReflection().className(dst);

  if(tipo.equals("String")){
    table=TableDef().add(key,C_TEXT,1024).create().addNew();
    bind(key,table,0,flag);
    ((String*)dst)->set(table.getStr(0));
    goto FIM;
  }
  if(tipo.equals("Number")){
    table=TableDef().add(key,C_DOUBLE).create().addNew();
    bind(key,table,0,flag);
    ((Number*)dst)->operator =(table.getDbl(0));
    goto FIM;
  }
  if(tipo.equals("Date")){
    table=TableDef().add(key,C_DATETIME).create().addNew();
    bind(key,table,0,flag);
    ((Date*)dst)->operator =(table.getDbl(0));
    goto FIM;
  }
  cPrintf("ERRO: PhpBind::bind [%s]",tipo.ptr());
  exit(1);
  FIM:
  return *this;
}
/*=========================================================

=========================================================*/
PhpBind &PhpBind::bind(cchar *key,Table &table,cchar *field,Table &src,cchar *fieldSrc){
  Table
    copia=src.copy().sort(fieldSrc);
  String
    value=getMap().get(key).asStr();
  int
    x=copia.find(fieldSrc,&value);

  table.putInt(
    field?field:key,
    !FOUND(x)?0:copia.go(x).getInt("id")
  );
  return *this;
}
