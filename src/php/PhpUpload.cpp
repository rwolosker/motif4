/*=========================================================

=========================================================*/
PhpUpload::PhpUpload(){
  initPhpUpload();
}
PhpUpload::~PhpUpload(){
}
void PhpUpload::initPhpUpload(void){
  length=0;
}
/*=========================================================

=========================================================*/
PhpUploadFile &PhpUpload::get(int x){
  if(x>=0&&x<length)
    return *(PhpUploadFile*)gco.get(x);
  else{
    PhpUploadFile
      &file=*PhpUploadFile().instance();

    nulo.add(&file);
    return file;
  }
}
/*=========================================================

=========================================================*/
PhpUpload &PhpUpload::remove(int x){
  PhpUploadFile
    &f=get(x);
  int
    antes=gco.len(),
    depois=0;

  gco.remove(&f);
  depois=gco.len();
  if(antes!=depois)
    length-=1;
  return *this;
}
/*=========================================================

=========================================================*/
int PhpUpload::len(void){
  return length;
}
/*=========================================================

=========================================================*/
PhpUpload &PhpUpload::load(cchar *name){
  length=0;
  gco.reset();
  for(int i=0;;i++){
    MapItem
      &item=getMap().get(String().sprintf("%s%d",name,i));

    if(item.isNull())
      break;
    else{
      String
        url=item.asStr();
      int
        x=url.inStr(";base64,");

      if(FOUND(x)){
        PhpUploadFile
          &file=*PhpUploadFile().instance();
        Split
          item=Split(url.mid(0,x),':');

        file.name=item[0];
        file.type=item[2];
        file.base64.writeStr(url.mid(x+8)).seek(0);
        file.binary=Base64().decode(file.base64.toString());

        length+=1;
        gco.add(&file);
      }
    }
  }
  return *this;
}
/*=========================================================

=========================================================*/
PhpUpload &PhpUpload::add(cchar *name,ByteArray &buffer){
  PhpUploadFile
    &file=*PhpUploadFile().instance();

  file.name=name;
  file.type="application/octet-stream";
  file.binary=buffer;
  file.base64.writeStr(Base64().encode(buffer)).seek(0);

  length+=1;
  gco.add(&file);

  return *this;
}
/*=========================================================

=========================================================*/
PhpUpload &PhpUpload::reset(void){
  gco.reset();
  initPhpUpload();
  return *this;
}
/*=========================================================

=========================================================*/
PhpUploadFile::PhpUploadFile(){}
PhpUploadFile::~PhpUploadFile(){}
/*=========================================================

=========================================================*/
PhpUploadFile *PhpUploadFile::instance(void){
  return new(cMemAlloc(sizeof(PhpUploadFile)))PhpUploadFile();
}
/*=========================================================

=========================================================*/
String PhpUploadFile::toUrl(void){
  if(!base64.len())
    base64.writeStr(Base64().encode(binary)).seek(0);
  return String()
    .append(name)
    .append(":data:")
    .append(type)
    .append(";base64,")
    .append(base64.toString());
}
