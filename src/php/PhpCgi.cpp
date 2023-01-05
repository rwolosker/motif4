/*=========================================================

=========================================================*/
PhpCgi::PhpCgi(){
  initPhpCgi();
}
PhpCgi::~PhpCgi(){

}
void PhpCgi::initPhpCgi(void){

}
/*=========================================================

=========================================================*/
ByteArray &PhpCgi::getDebugBuffer(void){return debugBuffer;}
ByteArray &PhpCgi::getEvalBuffer(void){return evalBuffer;}
ByteArray &PhpCgi::getOutBuffer(void){return outBuffer;}
/*=========================================================

=========================================================*/
PhpCgi &PhpCgi::main(void){
  openFile();
  inDump();
  return *this;
}
/*=========================================================

=========================================================*/
void PhpCgi::openFile(void){
  class DoIt{
    public:
    char open(Map &map,const String &name,File &f){
      MapItem
        &item=map.get(PSTRING(name));

      if(item.isNull())
        return 0;
      if(!f.access(item.asStr(),"rw-"))
        return 0;
      f.open(item.asStr(),0);
      return f.isOpen();
    }
  };
  DoIt
    doIt;

  doIt.open(getMap(),"in",inFile);
  doIt.open(getMap(),"out",outFile);
  doIt.open(getMap(),"debug",debugFile);
  doIt.open(getMap(),"eval",evalFile);
}
/*=========================================================

=========================================================*/
void PhpCgi::inDump(void){
  ByteArray
    buffer;

  inFile.readAll(buffer);
  if(getMap().get("php.cgi.openFile.keep").asInt()){
    File
      f;

    f.open("/tmp/in.txt");
    f.truncate();
    f.write(buffer);
  }
  buffer.seek(0);
  while(!buffer.eof()){
    int
      keyLen,valueLen;
    String
      key,value;

    keyLen=buffer.readInt();
    key=buffer.readStr(keyLen);
    valueLen=buffer.readInt();
    value=buffer.readStr(valueLen);

    getMap().setValue(key,value);
  }
}
/*=========================================================

=========================================================*/
PhpCgi &PhpCgi::flush(void){
  outFile.write(outBuffer);
  if(debugBuffer.len()){
    debugFile.write("\n<pre>\n");
    debugFile.write(debugBuffer);
  }
  evalFile.write(evalBuffer);

  outFile.sync();
  debugFile.sync();
  evalFile.sync();
  return *this;
}
/*=========================================================

=========================================================*/
PhpCgi &PhpCgi::debug(char debug,cchar *format,...){
  if(debug)
    debugBuffer.writeStr(
      *(String*)(String().squeeze(cVsprintf(&format)).me())
    );
  return *this;
}
/*=========================================================

=========================================================*/
PhpCgi &PhpCgi::eval(cchar *format,...){
  evalBuffer.writeStr(
    *(String*)(String().squeeze(cVsprintf(&format)).me())
  );
  return *this;
}
/*=========================================================

=========================================================*/
PhpCgi &PhpCgi::printf(cchar *format,...){
  outBuffer.writeStr(
    *(String*)(String().squeeze(cVsprintf(&format)).me())
  );
  return *this;
}
