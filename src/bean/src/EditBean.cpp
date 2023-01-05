/**********************************************************

**********************************************************/
EditBean::EditBean(){};
EditBean::~EditBean(){};
/*=========================================================

=========================================================*/
void EditBean::open(Map &map){
  String
    nome=getStr("editFile");
  File
    file;

  if(!file.access(nome,"r--"))
    printf("ERRO: Não achei este arquivo.");
  else{
    ByteArray
      buffer;

    file.open(nome,1);
    file.readAll(buffer);
    printf("%s",buffer.toString().ptr());
  }
}
/*=========================================================

=========================================================*/
void EditBean::save(Map &map){
  String
    nome=getStr("editFile"),
    body=getStr("editBody");
  File
    file;

  file.open(nome);
  if(!file.isOpen())
    printf("ERRO: Não consegui criar/abrir para escrever.");
  else{
    file.truncate();
    file.write(body);
    printf("Realizado com Sucesso.");
  }
}
