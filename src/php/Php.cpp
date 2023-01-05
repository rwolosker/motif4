/*=========================================================

=========================================================*/
Php::Php(){
  initPhp();
}
Php::~Php(){
}
void Php::initPhp(void){
  map=0;
  reflection=0;
}
/*=========================================================

=========================================================*/
Factory &Php::getFactory(void){return factory;}
Reflection &Php::getReflection(void){return *reflection;}
Php &Php::configure(Map &map){this->map=&map;return *this;}
GcObject &Php::getGcObject(void){return gco;}
/*=========================================================

=========================================================*/
Map &Php::getMap(void){
  if(!map){
    map=new(cMemAlloc(sizeof(Map)))Map();
    gco.add(map);
  }
  return *map;
}
/*=========================================================

=========================================================*/
Php &Php::setVersion(const String &version){
  this->version=version;
  return *this;
}
/*=========================================================

=========================================================*/
Php &Php::main(int argc,char **argv){
  Map
    &map=getMap();

  map.loadArg(argc,argv);
  map.setValue("app.path",map.get("arg0").asStr());
  iniConfig(map);
  reflectionConfig(map);
  factoryConfig(map);
  map.setValue("app.php",this);
  map.setValue("app.gco",&gco);
  map.setValue("appVersion",version);

  ssl32();

  map.protectAll().protectMode(1);
  cgiMain();
  map.protectMode(0);

  loadResource();
  loadTag();
  loadDat();
  actionMain();
  cgiFlush();
  return *this;
}
/*=========================================================

=========================================================*/
void Php::cgiFlush(void){
  PhpCgi
    *cgi;

  getMap().get("app.php.cgi").pObj(&cgi);
  cgi->flush();
}
/*=========================================================

=========================================================*/
void Php::actionMain(void){
  PhpAction
    *action;

  factory.getBean(
    "app.php.action",
    getMap().get("php.action.class").asStr(),
    &action
  );
  action->main();
}
/*=========================================================

=========================================================*/
void Php::loadDat(void){
  factory.getBean(
    "app.php.dat",
    getMap().get("php.dat.class").asStr()
  );
}
/*=========================================================

=========================================================*/
void Php::loadTag(void){
  factory.getBean(
    getMap().get("php.tag.name").asStr(),
    getMap().get("php.tag.class").asStr()
  );
}
/*=========================================================

=========================================================*/
void Php::loadResource(void){
  factory.getBean(
    "app.php.resource",
    getMap().get("php.resource.class").asStr()
  );
}
/*=========================================================

=========================================================*/
void Php::cgiMain(void){
  PhpCgi
    *cgi;

  factory.getBean(
    "app.php.cgi",
    getMap().get("php.cgi.class").asStr(),
    &cgi
  );
  cgi->main();
}
/*=========================================================

=========================================================*/
void Php::iniConfig(Map &map){
  MapItem
    &ini=map.get("ini");

  if(!ini.isNull())
    map.loadIni(ini.asStr());
}
/*=========================================================

=========================================================*/
void Php::reflectionConfig(Map &map){
  String
    appPath=map.get("app.path").asStr(),
    mapa=appPath.copy().append(".map");
  Split
    argv;
  char
    useTable=map.get("reflection.table.use").asInt();

  if(!useTable)
    reflection=new(cMemAllocChar(sizeof(Reflection)))Reflection();
  else{
    char
      reload=map.get("reflection.table.reload").asInt();

    reflection=new(cMemAllocChar(sizeof(ReflectionTable)))ReflectionTable();
    ((ReflectionTable*)reflection)->setReload(reload);
  }
  gco.add(reflection);

  argv.add(appPath);
  if(map.get("reflection.fileMap").asInt())
    reflection->fileMap(mapa);
  reflection->main(1,argv.ptr());
  map.setValue("app.reflection",&reflection);
}
/*=========================================================

=========================================================*/
void Php::factoryConfig(Map &map){
  factory.configure(map,*reflection);
  map.setValue("app.factory",&factory);
}
/*=========================================================

=========================================================*/
void Php::ssl32(void){
  SSL32
    *ssl;
  char
    ok;
  int
    security=getMap().get("app.security").asInt();

  if(!security)
    return;

  ssl=(SSL32*)getFactory().getBean(
    "app.ssl32",
    getMap().get("ssl32.class").asStr()
  );
  ok=ssl->init(
    getMap().get("ssl32.lib.crypto").asStr(),
    getMap().get("ssl32.lib.ssl").asStr()
  );
  if(!ok){
    cPrintf("ERRO: ssl32.");
    exit(1);
  }
  ssl->setKey(getMap().get("ssl32.key").asStr());
}
/*=========================================================

=========================================================*/
Php &Php::mapParse(Map &map){
  Split
    key=map.keys();
  WeParse
    parse;
  Factory
    *factory;

  map.get("app.factory").pObj(&factory);
  parse.setMap(map);
  parse.setFactory(*factory);
  parse.configure(map);

  for(int i=0;i<key.len();i++){
    MapItem
      &item=map.get(key[i]);

    if(item.getType()==C_PCHAR){
      String
        value=item.asStr().replace('$','#');

      parse.parse(value);
      map.setValue(key[i],value);
    }
  }
  return *this;
}
