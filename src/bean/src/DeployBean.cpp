/*=========================================================

=========================================================*/
DeployBean::DeployBean(){}
DeployBean::~DeployBean(){}
/*=========================================================

=========================================================*/
void DeployBean::unpacked(PhpUploadFile &item,File &flog){
  File
    f;
  String
    linha,cdir;
  Shell
    exe;
  ByteArray
    buffer;

  getcwd(cdir.resize(1024),1024);
  f.open("/tmp/deploy.zip");
  f.truncate();
  f.write(item.binary);
  f.temp();
  exe.exec("unzip -Z1 /tmp/deploy.zip *.zip",f);
  f.readAll(buffer);
  while(buffer.readLine(linha)){
    String
      arquivo=linha,
      dir=getStr(arquivo.mid(0,arquivo.len()-4));

    if(!dir.len()){
      flog.write(String().sprintf(
        "ERRO: %s\n",
        arquivo.ptr())
      );
      continue;
    }
    chdir(cdir);
    chdir(dir);
    exe.exec(String().sprintf(
      "pwd\n"
      "unzip -p /tmp/deploy.zip %s > %s\n"
      "unzip -o -d . %s\n",
      arquivo.ptr(),
      arquivo.ptr(),
      arquivo.ptr()),
      flog
    );
    chdir(cdir);
  }
  flog.sync();
}
/*=========================================================

=========================================================*/
void DeployBean::install(Map &map){
  inject(&arquivo);
  setObj("arquivo",&arquivo);
  arquivo.load("arquivo");
  for(int i=0;i<arquivo.len();i++){
    PhpUploadFile
      &item=arquivo.get(i);

    if(item.name.inStrRev("zip")==item.name.len()-3){
      File
        flog;
      ByteArray
        buffer;

      flog.temp();
      unpacked(item,flog);
      flog.readAll(buffer);
      arquivo.reset();
      arquivo.add("deploy.log",buffer);
      message(MSG_SUCCESS,"Realizado com Sucesso.");
      break;
    }
  }

  dispatch("admin/deploy/deploy.html");
}
/*=========================================================

=========================================================*/
void DeployBean::make(Map &map){
  File
    f;
  ByteArray
    b;

  f.temp();
  packed(f,b);
  inject(&arquivo);
  setObj("arquivo",&arquivo);
  arquivo.add("deploy.zip",b);
  f.readAll(b.reset());
  arquivo.add("deploy.log",b);
  datType();
  message(MSG_SUCCESS,"Realizado com Sucesso.");
  dispatch("admin/deploy/deploy.html");
}
/*=========================================================

=========================================================*/
void DeployBean::packed(File &f,ByteArray &b){
  Split
    item=Split(getMap().get("site").asStr(),',');
  String
    cdir;
  Shell
    exe;

  b.reset();
  getcwd(cdir.resize(1024),1024);
  exe.exec(
    "rm -rfv /tmp/deploy.zip\n",
    f
  );

  for(int i=0;i<item.len();i++){
    String
      dir=getMap().get(item[i]).asStr();

    chdir(cdir);
    chdir(dir);
    exe.exec(String().sprintf(
      "pwd\n"
      "rm -rfv %s.zip\n"
      "zip -r %s.zip *\n"
      "unzip -l %s.zip\n"
      "zip /tmp/deploy.zip %s.zip\n"
      "unzip -l /tmp/deploy.zip\n"
      "rm -rfv %s.zip\n",
      item[i].ptr(),
      item[i].ptr(),
      item[i].ptr(),
      item[i].ptr(),
      item[i].ptr()),
      f
    );
  }
  chdir(cdir);
  {
    File
      f;

    f.open("/tmp/deploy.zip");
    f.readAll(b);
    f.unlink();
  }
}
/*=========================================================

=========================================================*/
void DeployBean::datType(void){
  PhpDat
    dat;
  Split
    list;
  ByteArray
    buffer;
  TableStruct
    ts;

  inject(&dat);
  dat.listTables(list);
  for(int i=0;i<list.len();i++)
    ts.source(dat.use(list[i]),buffer);
  arquivo.add("datType.h",buffer);
}
