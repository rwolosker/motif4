/*=========================================================

=========================================================*/
PhpAudit::PhpAudit(){
}
PhpAudit::~PhpAudit(){
}
/*=========================================================

=========================================================*/
PhpAudit *PhpAudit::instance(void){
  return new(cMemAlloc(sizeof(PhpAudit)))PhpAudit();
}
/*=========================================================

=========================================================*/
PhpAudit &PhpAudit::log(Table &table,cchar *id,cchar *label){
  int
    recno=table.recNo();
  Table
    reg=table.copy().removeAll(recno);
  ByteArray
    stream;
  Base64
    base64;
  String
    userId=getMap().get("app.user.id").asStr();
  Date
    data;
  File
    file;

  reg.toStream(stream);
  stream=base64.encode(stream).ptr();
  data.now();
  file.open(getMap().get("php.audit.log").asStr());
  file.seek(file.size());

  file.write(String().sprintf(
    "%s;%s;%s;%d;%s\n",
    data.format().ptr(),
    userId.ptr(),
    label,
    reg.getInt(id),
    stream.toString().ptr()
  ));

  return *this;
}
/*=========================================================

=========================================================*/
PhpAudit &PhpAudit::toMap(cchar *name){
  getMap().setValue(name,this,1);
  return *this;
}
/*=========================================================

=========================================================*/
String PhpAudit::audit(cchar *panelId,cchar *dataSec,int id,cchar *label){
  BEANMAP(PhpResource,res,"app.php.resource")
  BEANMAP(PhpUtil,util,"app.php.util")
  String
    html;
  Map
    &map=getMap();

  res.load("admin/audit.html",html);
  map.setValue("auditTag",label);
  map.setValue("auditId",id);
  map.setValue("auditPanelId",panelId);
  map.setValue("auditSec",dataSec);
  res.parse(html);

  if(!id||!util.hasRole(dataSec))
    html="";
  return html;
}
/*=========================================================

=========================================================*/
void PhpAudit::table(Map &map){
  BEANMAP(PhpDat,dat,"app.php.dat")
  BEANMAP(PhpCgi,cgi,"app.php.cgi")
  String
    label=map.get("auditTag").asStr(),
    id=map.get("auditId").asStr(),
    linha;
  File
    file;
  ByteArray
    log,res;
  Table
    usuario=dat.use("usuario");
  char
    colunaNeed=1;
  Split
    coluna;

  file.open(
    map.get("php.audit.log").asStr(),1
  );
  file.readAll(log);
  log.seek(0);
  while(log.readLine(linha)){
    Split
      item=Split(linha,';');

    if(!item[2].equals(label))
      continue;
    if(!item[3].equals(id))
      continue;
    {
      Date
        data=Date().parse(item[0]);
      Number
        usuarioId=Number().parse(item[1]);
      Base64
        base64;
      Table
        table;
      ByteArray
        stream;
      int
        x,erro;
      String
        matricula;

      x=usuario.find("id",&usuarioId);
      if(FOUND(x)){
        matricula=usuario.go(x).getStr("matricula");
        if(!matricula.len())
          matricula=usuario.getStr("cpf");
      }

      stream=base64.decode(item[4]);
      table.fromStream(stream,&erro).go(0);
      res.writef(
        "<tr><td>%s</td><td>%s</td>",
        data.format().ptr(),
        matricula.ptr()
      );
      {
        TableDef
          &def=table.getDef();

        for(int i=0;i<def.len();i++){
          Field
            &f=def.get(i);

          if(colunaNeed)
            coluna.add(f.getName());
          res.writef(
            "<td>%s</td>",
            f.getString().ptr()
          );
        }
        res.writeStr("</tr>");
      }
    }
    colunaNeed=0;
  }
  res=ByteArray()
    .writeStr("<table><tr><th>data</th><th>user</th><th>")
    .writeStr(coluna.join("</th><th>"))
    .writeStr("</th></tr>")
    .writeStr(res.toString())
    .writeStr("</table>");

  inverte(res);
  cgi.printf("%s",res.toString().ptr());
}
/*=========================================================

=========================================================*/
void PhpAudit::inverte(ByteArray &buffer){
  String
    tmp,
    cabecalho;
  Split
    item;

  tmp=buffer
    .toString()
    .replace("</tr>",'\n')
    .replace("<table>","")
    .replace("</table>","");
  buffer.reset().writeStr(tmp);

  buffer.seek(0);
  buffer.readLine(cabecalho);
  while(buffer.readLine(tmp))
    item.add(tmp);

  buffer
    .reset()
    .writeStr("<table>")
    .writeStr(cabecalho)
    .writeStr("</tr>");
  for(int i=item.len()-1;i>=0;i--)
    buffer
      .writeStr(item[i])
      .writeStr("</tr>");
  buffer.writeStr("</table>");
}
