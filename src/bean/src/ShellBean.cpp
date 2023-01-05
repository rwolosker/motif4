ShellBean::ShellBean(){};
ShellBean::~ShellBean(){};
/*=========================================================

=========================================================*/
void ShellBean::upload(Map &map){
  PhpUpload
    upload;

  inject(&upload);
  upload.load("file");
  if(!upload.len())
    printf("ERRO: Nenhum arquivo recebido.");
  else{
    PhpUploadFile
      &item=upload.get(0);
    File
      file;

    file.open(item.name);
    if(!file.isOpen()){
      printf("ERRO: ao criar [%s].",item.name.ptr());
      return;
    }
    file.write(item.binary);
    printf("Realizado com Sucesso.");
  }
}
/*=========================================================

=========================================================*/
void ShellBean::download(Map &map){
  String
    file=getStr("file");

  if(!file.len()){
    printf("ERRO: Não tem nada.");
    return;
  }
  else{
    File
      f;

    f.open(file,1);

    if(f.isDir()||!f.access(file,"---")||!f.isOpen(file,1)){
      printf("ERRO: Não achei o arquivo ou é um diretório [%s].",file.ptr());
      return;
    }
    else{
      ByteArray
        buffer;
      PhpUpload
        upload;

      inject(&upload);
      f.readAll(buffer);
      printf("%s",upload
        .add(f.getName(1),buffer)
        .get(0)
        .toUrl()
        .ptr());
    }
  }
}
/*=========================================================

=========================================================*/
void ShellBean::execute(Map &map){
  Shell
    shell;
  String
    command=getStr("command").replace('\r',"");
  ByteArray
    buffer;
  File
    file;

  file.temp();
  shell.exec(command,file);
  file.readAll(buffer);
  setStr("commandOut","%s",buffer.toString().ptr());
  dispatch("admin/shell/shell.html");
}
/*=========================================================

=========================================================*/
void ShellBean::executeAjax(Map &map){
  Shell
    shell;
  String
    command=getStr("command").replace('\r',"");
  ByteArray
    buffer;
  File
    file;

  file.temp();
  shell.exec(command,file);
  file.readAll(buffer);

  printf("%s\n",buffer.toString().ptr());
}
