/*=========================================================

=========================================================*/
void ActionResolve::csvLoad(Table &table){
  int
    reload=getMap().get("php.action.resolve.csv.reload").asInt();

  if(reload||!table.getRows()){
    File
      f;
    ByteArray
      buffer;
    String
      linha;
    int
      id=0;

    linha=getMap().get("php.action.resolve.csv").asStr();
    f.open(
      linha,
      1
    );

    f.readAll(buffer);
    table.removeAll();
    while(buffer.readLine(linha)){
      Split
        item=Split(linha,';');

      table
        .addNew()
        .putInt("id",++id)
        .putTxt("alias",item[0])
        .putTxt("real",item[1])
        .putInt("ativo",1);
    }
    table.sort("alias");
    table.save();
  }
}
ActionResolve::ActionResolve(){}
ActionResolve::~ActionResolve(){}
/*=========================================================

=========================================================*/
char ActionResolve::resolve(String &action,String &type){
  BEANMAP(PhpDat,dat,"app.php.dat")
  Table
    &alias=dat.use("alias");
  int
    x;

  csvLoad(alias);
  x=alias.find("alias",action.me());
  if(FOUND(x)){
    alias.go(x);
    if(!alias.getInt("ativo"))
      return 0;
    action=alias.getStr("real");
    return 1;
  }
  return 0;
}

