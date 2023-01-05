/*=========================================================

=========================================================*/
PhpResource::PhpResource(){
}
PhpResource::~PhpResource(){
}
/*=========================================================

=========================================================*/
String PhpResource::find(cchar *file){
  Split
    item=Split().split(getMap().get("site").asStr(),',');
  File
    f;
  String
    test;

  for(int i=0;i<item.len();i++){
    test=getMap().get(item[i]).asStr().append('/').append(file);
    if(f.access(test,"r--"))
      return test;
  }
  return "";
}
PhpResource &PhpResource::loadStream(cchar *file,ByteArray &buffer){
  File
    f;

  buffer.reset();
  if(f.isOpen(find(file),1))
    f.readAll(buffer);
  return *this;
}
/*=========================================================

=========================================================*/
PhpResource &PhpResource::load(cchar *file,String &buffer){
  ByteArray
    b;

  loadStream(file,b);
  buffer=b.toString();
  return *this;
}
/*=========================================================

=========================================================*/
PhpResource &PhpResource::send(String &buffer){
  PhpCgi
    *cgi;

  getMap().get("app.php.cgi").pObj(&cgi);
  cgi->getOutBuffer().writeStr(buffer);
  return *this;
}
/*=========================================================

=========================================================*/
PhpResource &PhpResource::parse(String &buffer){
  GcObject
    *gco;
  WeParse
    *parse;

  getMap().get("app.gco").pObj(&gco);
  getFactory().getBean(0,"WeParse",&parse);
  parse->parse(buffer);
  gco->add(parse);
  return *this;
}

