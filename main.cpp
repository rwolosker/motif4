/**********************************************************
bash setup.sh  runHttpSSL2
**********************************************************/
#include "../cepat/src/core.h"
#include "../cepat/src/webengine/engine.h"
#include "../cepat/src/sk32/sk32.h"
#include "../cepat/src/ldap/ldap.h"
#include "../cepat/src/dat/dat.h"
#include "../cepat/src/reflectionTable/reflection.h"
#include "../cepat/src/tds/tds.h"
#include "../cepat/src/mysql/mysql.h"
#include "../cepat/src/sqlite/SqLite.h"
#include "src/motif.h"
/*=========================================================

=========================================================*/
void Main(int argc,char **argv){
  Php
    php;

  if(0){
    chdir("/home/ricardo/temp/heliosworkspace/motif4/site");
  }
  php.main(argc,argv);
}
/*=========================================================

=========================================================*/
void teste(void){
  PhpDat
    dat;
  Map
    map;
  Split
    list;
  Table
    res;

  map.setValue("dat","motif.dat");
  map.setValue("motif.dat","dat");
  dat.setMap(map);

  list.add("usuario");
  list.add("grupo");
  list.add("usuarioGrupo");

  for(int i=0;i<list.len();i++){
    Table
      tmp=dat.use(list[i]),
      join;
    TableDef
      def;

    for(int c=0;c<tmp.getDef().len();c++){
      String
        name=tmp.getDef().get(c).getName();

      tmp.getDef().rename(name,tmp.getName().append(".").append(name));
    }
    if(!i){
      res=tmp;
      continue;
    }
    for(int c=0;c<res.getDef().len();c++){
      Field
        &f=res.getDef().get(c);

      def.add(
        f.getName(),
        f.getType(),
        f.getSize()
      );
    }
    for(int c=0;c<tmp.getDef().len();c++){
      Field
        &f=tmp.getDef().get(c);

      def.add(
        f.getName(),
        f.getType(),
        f.getSize()
      );
    }
    join=def.create();
    res.top();
    while(res.fetch()){
      tmp.top();
      while(tmp.fetch())
        join.addReg(&res,&tmp,0);
    }
    res=join;
  }
  res.debug();
}
/*=========================================================

=========================================================*/
int main(int argc,char **argv){
  //teste();
  Main(argc,argv);
	return cMemUsed();
}
