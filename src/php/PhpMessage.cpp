/*=========================================================

=========================================================*/
PhpMessage::PhpMessage(){
  initPhpMessage();
}
PhpMessage::~PhpMessage(){
}
void PhpMessage::initPhpMessage(void){
  time=0;
}
/*=========================================================

=========================================================*/
PhpMessage &PhpMessage::setTime(int value){
  getMap().setValue("AspMessageTime",(time=value)*1000);
  return *this;
}
/*=========================================================

=========================================================*/
PhpMessage &PhpMessage::setMessage(int type,cchar *format,...){
  String
    tipo;

  if(!time)
    time=10;

  switch(type){
    case MSG_ALERT:
      tipo="alert";break;
    case MSG_INFO:
      tipo="info";break;
    case MSG_ERROR:
      tipo="error";break;
    case MSG_SUCCESS:
      tipo="success";break;
  }

  getMap().setValue("AspMessage",String().squeeze(cVsprintf(&format)));
  getMap().setValue("AspMessageType",tipo);
  getMap().setValue("AspMessageTime",time*1000);

  return *this;
}
