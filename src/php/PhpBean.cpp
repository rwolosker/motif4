/*=========================================================

=========================================================*/
PhpBean::PhpBean(){
  initPhpBean();
}
PhpBean::~PhpBean(){
}
void PhpBean::initPhpBean(void){
  cgi=0;
  php=0;
  msg=0;
}
Php &PhpBean::getPhp(void){return *php;}
PhpCgi &PhpBean::getCgi(void){return *cgi;}
/*=========================================================

=========================================================*/
PhpMessage &PhpBean::getMsg(void){
  if(msg)
    return *msg;
  else{
    getFactory().getBean("app.php.message","PhpMessage",&msg);
    return *msg;
  }
}
/*=========================================================

=========================================================*/
void PhpBean::configure(Map &map){
  getMap().get("app.php").pObj(&php);
  getMap().get("app.php.cgi").pObj(&cgi);
}
/*=========================================================

=========================================================*/
String PhpBean::fileLoad(cchar *fileName){
  BEANMAP(PhpResource,resource,"app.php.resource")
  String
    buffer;

  resource.load(fileName,buffer);
  return buffer;
}
/*=========================================================

=========================================================*/
void PhpBean::inject(Object *obj){
  getFactory().inject(obj);
}
/*=========================================================

=========================================================*/
void PhpBean::debug(char debug,cchar *format,...){
  if(!debug)
    return;
  else{
    String
      value=String().squeeze(cVsprintf(&format));

    getCgi().getDebugBuffer().writeStr(value.append("\n"));
  }
}
/*=========================================================

=========================================================*/
void PhpBean::eval(cchar *format,...){
  String
    value=String().squeeze(cVsprintf(&format));

  getCgi().getEvalBuffer().writeStr(value.append("\n"));
}
/*=========================================================

=========================================================*/
void PhpBean::printf(cchar *format,...){
  String
    value=String().squeeze(cVsprintf(&format));

  getCgi().getOutBuffer().writeStr(value);
}
/*=========================================================

=========================================================*/
String PhpBean::getStr(cchar *name){
  return getMap().get(name).asStr();
}
/*=========================================================

=========================================================*/
int PhpBean::getInt(cchar *name){
  return getMap().get(name).asInt();
}
/*=========================================================

=========================================================*/
void PhpBean::setStr(cchar *name,cchar *format,...){
  getMap().setValue(name,String().squeeze(cVsprintf(&format)));
}
/*=========================================================

=========================================================*/
void PhpBean::setObj(cchar *name,Object *obj,char free){
  getMap().setValue(name,obj,free);
}
/*=========================================================

=========================================================*/
void PhpBean::dispatch(cchar *fileName){
  BEANMAP(PhpResource,resource,"app.php.resource")
  String
    buffer;

  resource.load(fileName,buffer);
  resource.parse(buffer);
  resource.send(buffer);
}
/*=========================================================

=========================================================*/
PhpMessage &PhpBean::message(int type,char *format,...){
  return getMsg().setMessage(
    type,
    "%s",String().squeeze(cVsprintf(&format)).ptr()
  );
}
/*=========================================================

=========================================================*/
void PhpBean::debugMap(void){
  File
    f;
  int
    old;
  String
    buffer;

  f.temp();
  old=cStdout;
    cStdout=f.getF();
    getMap().debug();
    f.readAll(buffer);
    debug(1,"%s\n",buffer.ptr());
  cStdout=old;
}
/*=========================================================

=========================================================*/
void PhpBean::debugTableReg(Table &reg){
  for(int i=0;i<reg.getDef().len();i++)
    debug(
     1,
     "%s=%s",
     reg.getDef().get(i).getName().ptr(),
     reg.getStr(i).ptr()
    );
}
/*=========================================================

=========================================================*/
void PhpBean::debugTable(Table &table,char dump){
  File
    f;
  int
    old;
  String
    buffer;
  void
    *pold;

  f.temp();
  old=cStdout;
  pold=cHexdumpPrint;
    cStdout=f.getF();
    cHexdumpPrint=VOID(cPrintf);
    table.debug(dump);
    f.readAll(buffer);
    debug(1,"%s\n",buffer.ptr());
  cStdout=old;
  cHexdumpPrint=pold;
}
/*=========================================================

=========================================================*/
void PhpBean::injectAllImpl(void *p){
  void
    **pp=(void**)p;
  int
    istart=0;

  cGnuStart(&istart);
  for(int i=istart;pp[i];i++)
    inject((Object*)pp[i]);

}
/*=========================================================

=========================================================*/
void PhpBean::injectAll(...){
  char
    *p=0;

  injectAllImpl(&p);
}
