/*=========================================================

=========================================================*/
PhpUtil::PhpUtil(){
}
PhpUtil::~PhpUtil(){
}
/*=========================================================

=========================================================*/
char *PhpUtil::strNull(cchar *value){
  return PCHAR(value?value:"");
}
/*=========================================================

=========================================================*/
String PhpUtil::strInput(const String &value,char capitalize){
  String
    tmp=value;
  char
    *p=tmp.ptr(),
    *a=p;

  while(p&&*p){
    switch(*p){
      case '#':case '{' :case '}':case '(' :case ')':case '$':
      case '%':case '~' :case '^':case '*' :case '+':case '-':
      case '|':case ';' :case ':':case '?' :case '!':case '>':
      case '<':case '\'':case '"':case '\t':
        *a++=' ';
        break;
      default:
        *a++=*p;
    }
    p++;
  }
  *a=0;
  tmp=tmp.replace("  ",' ').trim();
  if(capitalize){
    Split
      list;
    uchar
      *ucase=PUCHAR(CONV_ISO_8859_15_UCASE),
      **v;

    list=Split(tmp.lCase(),' ');
    v=(uchar**)list.ptr();
    while(v&&*v){
      uchar
        *c=*v++;

      *c=ucase[*c];
    }
    tmp=list.join(' ');
  }
  return tmp;
}
/*=========================================================

=========================================================*/
String PhpUtil::dteInput(const String &value){
  String
    tmp=value;
  char
    *p=tmp.ptr(),
    *a=p;

  while(p&&*p){
    if(*p>='0'&&*p<='9'){
      *a++=*p++;
      continue;
    }
    switch(*p){
      case ' ':
      case ':':
      case '/':
        *a++=*p++;
        continue;
    }
    p++;
  }

  return tmp.replace("  ",' ').trim();
}
/*=========================================================

=========================================================*/
String PhpUtil::numInput(const String &value){
  String
    tmp=value;
  char
    *p=tmp.ptr(),
    *a=p;

  while(p&&*p){
    if(*p>='0'&&*p<='9'){
      *a++=*p++;
      continue;
    }
    switch(*p){
      case '-':
      case '.':
      case ',':
        *a++=*p++;
        continue;
    }
    p++;
  }
  if(a)
    *a=0;

  return tmp;
}
/*=========================================================

=========================================================*/
String PhpUtil::format(Table &table,cchar *field){
  Field
    &f=table.getDef().get(field);

  switch(f.getType()){
    case C_BOOL:
      return f.getInt()?"SIM":"NÃO";
    case C_DATE:
      return f.getDate().format("dd/MM/yyyy");
    case C_DATETIME:
      return f.getDate().format("dd/MM/yyyy HH:mm:ss");
    default:
      return strNull(f.getString());
  }
}
/*=========================================================

=========================================================*/
char PhpUtil::isHexNumber(const String &value){
  String
    tmp=PSTRING(value).uCase();
  char
    *p=tmp.ptr();

  if(!p||!*p)
    return 0;
  while(p&&*p){
    char
      ok=0
        ||(*p>='0'&&*p<='9')
        ||(*p>='A'&&*p<='F');

    if(!ok)
      return 0;
    p++;
  }
  return 1;
}
/*=========================================================

=========================================================*/
char PhpUtil::checkBox(cchar *name){
  MapItem
    item=getMap().get(name);

  return !item.isNull();
}
/*=========================================================

=========================================================*/
void PhpUtil::checkList(cchar *name,Split &out){
  Map
    &map=getMap();
  int
    len=map.get(String(name).append("Len")).asInt();

  out.reset();
  for(int i=0;i<len;i++){
    MapItem
      item=map.get(String(name).appendf("%d",i));

    if(!item.isNull())
      out.add(item.asStr());
  }
}
/*=========================================================

=========================================================*/
String PhpUtil::ipRequest(void){
  String
    ip;
  MapItem
    *item;

  #define resolve(key)\
    item=&getMap().get(key);\
    if(!item->isNull()){\
      ip=item->asStr();\
      goto FIM;\
    }

  resolve("X-Forwarded-For")
  resolve("HTTP_X_FORWARDED_FOR")
  resolve("HTTP_CLIENT_IP")
  resolve("HTTP_X_TINYPROXY")
  resolve("REMOTE_ADDR")

  FIM:
  #undef resolve
  return ip;
}
/*=========================================================

=========================================================*/
int PhpUtil::userId(const String &key){
  MapItem
    &item=getMap().get(PSTRING(key));

  if(item.isNull())
    return 0;
  else{
    BEANMAP(SSL32,ssl,"app.ssl32")
    ByteArray
      buffer=item.asStr().ptr();

    ssl.decrypt(0,buffer);
    return Number().parse(Split(buffer.toString(),';')[0]);
  }
}
/*=========================================================
grupo,grupo
=========================================================*/
char PhpUtil::hasRole(const String &value){
  PhpLogin
    *login=(PhpLogin*)getMap().get("app.login").asPtr();

  if(!login)
    return 0;
  else{
    Split
      item=Split(PSTRING(value),',');

    for(int i=0;i<item.len();i++){
      int
        ok=login->getGrupo().search(&item[i].ref());

      if(FOUND(ok))
        return 1;
    }
  }
  return 0;
}
/*=========================================================
grupo,grupo
=========================================================*/
String PhpUtil::userAttr(const String &key){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    usuario=dat.use("usuario");
  int
    x=usuario.find("id",Number(userId()).me());

  if(!FOUND(x))
    return String();
  else
    return usuario.go(x).getStr(PSTRING(key));
}
/*=========================================================

=========================================================*/
String PhpUtil::fdouble(Table &table,cchar *field,int dec,char floatBr){
  return fdouble(table.getDbl(field),dec,floatBr);
}
String PhpUtil::fdouble(double value,int dec,char floatBr){
  String
    out=String().sprintf("%.*f",dec,value);

  if(floatBr)
    out=out.replace('.',',');
  return out;
}
String PhpUtil::fCpf(const String &value){
  String
    src=String().sprintf("%11s",PSTRING(value).ptr()).replace(' ','0'),
    dst=String().space(11+3);
  char
    *ps0=src.ptr(),
    *ps=ps0+src.len()-1,
    *pd0=dst.ptr(),
    *pd=pd0+dst.len()-1;

  while(ps>=ps0){
    switch(ps-ps0){
      case 8:
        *pd--='-';
        *pd--=*ps;
        break;
      case 5:
      case 2:
        *pd--='.';
        *pd--=*ps;
        break;
      default:
        *pd--=*ps;
    }
    ps--;
  }
  return dst;
}


