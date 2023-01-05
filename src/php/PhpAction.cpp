/*=========================================================

=========================================================*/
PhpAction::PhpAction(){
  initPhpAction();
}
PhpAction::~PhpAction(){
}
void PhpAction::initPhpAction(){
}
/*=========================================================

=========================================================*/
PhpAction &PhpAction::main(void){
  String
    requestUri,scriptName,action,type;
  int
    x;

  requestUri=getMap().get("REQUEST_URI").asStr();
  scriptName=getMap().get("SCRIPT_NAME").asStr();
  if(FOUND(x=requestUri.inStr('?')))
    requestUri[x]=0;
  action=requestUri.copy().replace(scriptName,"").mid(1);
  type=Split(action,'.').last();
  resolve(action,type);
  return *this;
}
/*=========================================================

=========================================================*/
char PhpAction::isPublico(String &action){
  BEANMAP(PhpDat,dat,"app.php.dat")
  TableRef
    publico=dat.use("publico");
  int
    x=publico.find("nome",&action);

  return 1
    &&FOUND(x)
    &&publico.go(x).getInt("ativo");
}
/*=========================================================

=========================================================*/
void PhpAction::resolve(String &action,String &type){
  String
    resolveClass=getMap().get("php.action.resolve.class").asStr(),
    resolveMethod=getMap().get("php.action.resolve.method").asStr(),
    loginClass=getMap().get("php.login.class").asStr(),
    securityClass=getMap().get("php.security.class").asStr(),
    original=action;

  Object
    *bean=0;
  char
    (Object::*resolve)(String&,String&)=0,
    publico=0;

  bean=getFactory().getBean("app.action.resolve",resolveClass);
  *(void**)&resolve=getReflection().methodAddress(resolveMethod);
  if(!(bean->*resolve)(action,type)){
    BEANMAP(PhpResource,res,"app.php.resource");

    res.send(String()
      .sprintf(
        "Não tem <u>alias</u> para isso: <b>%s</b>.",
        action.ptr(),
        type.ptr())
    );
    log(original,"");
    return;
  }
  log(original,action);
  publico=isPublico(action);
  if(!publico&&getMap().get("app.security").asInt()){
    BEAN(PhpMessage,msg,"app.php.message");
    PhpLogin
      &login=*(PhpLogin*)getFactory().getBean("app.login",loginClass);

    if(!login.fromCookie()){
      msg.setMessage(MSG_ERROR,"Autentique-se primeiro.");
      sendFile(String("admin/usuario/login.html").append(""));
      return;
    }
    else{
      PhpSecurity
        &security=*(PhpSecurity*)getFactory().getBean("app.security.bean",securityClass);

      if(!security.access(login,action,type)){
        msg.setMessage(MSG_ERROR,"Você não tem acesso. [<b>%s</b>].",action.ptr());
        sendFile(String("aplicacao.html").append(""));
        return;
      }
    }
  }

  if(type.equals("html")){
    sendFile(action);
    return;
  }
  if(type.equals("htm")){
    execute(action);
    return;
  }
}
/*=========================================================

=========================================================*/
void PhpAction::sendFile(String &action){
  PhpResource
    *resource;
  String
    buffer;

  getMap().get("app.php.resource").pObj(&resource);
  resource->load(action,buffer);
  if(!buffer.len())
    resource->send(String()
      .sprintf("Não sei o que fazer com isso: <b>%s</b>.",action.ptr()));
  else{
    resource->parse(buffer);
    resource->send(buffer);
  }
}
/*=========================================================

=========================================================*/
void PhpAction::execute(String &action){
  Split
    item=Split(action,"::");

  if(item.len()==2){
    String
      className=item[0],
      beanName=className.mid(0,1).lCase().append(className.mid(1)),
      method=action;
    Object
      *obj;
    void
      (Object::*pmethod)(Map &map)=0;

    obj=getFactory().getBean(beanName,className);
    *(void**)&pmethod=getReflection().methodAddress(method);

    if(obj&&pmethod)
      (obj->*pmethod)(getMap());
    else{
      BEANMAP(PhpResource,res,"app.php.resource");

      res.send(String()
      .sprintf("Não sei o que fazer com isso: <b>%s</b>.",action.ptr()));
    }
    return;
  }
}
/*=========================================================

=========================================================*/
void PhpAction::log(cchar *origem,cchar *action){
  BEAN(PhpUtil,util,"app.php.util")
  String
    fileName=getMap().get("php.action.log").asStr();
  File
    f;

  f.open(fileName);
  f.seek(f.size());
  f.write(String().sprintf(
    "%s;%s;%s;%s\n",
    Date().now().format().ptr(),
    util.ipRequest().ptr(),
    origem,
    action
  ));




}




