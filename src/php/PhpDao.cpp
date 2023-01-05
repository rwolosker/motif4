PhpDao::PhpDao(){
  initPhpDao();
}
PhpDao::~PhpDao(){
}
/*=========================================================

=========================================================*/
void PhpDao::initPhpDao(void){
  phpDat=0;
}
/*=========================================================

=========================================================*/
PhpDat &PhpDao::getDat(void){
  if(phpDat)
    return *phpDat;
  else{
    BEAN(PhpDat,dat,"app.php.dat")

    phpDat=&dat;
    return *phpDat;
  }
}
/*=========================================================

=========================================================*/
Table &PhpDao::useImpl(cchar *name){
  return getDat().use(name);
}
/*=========================================================

=========================================================*/
String PhpDao::serialize(Table &table){
  return getDat().serialize(table);
}
/*=========================================================

=========================================================*/
Table PhpDao::unserialize(cchar *value){
  return getDat().unserialize(value);
}
