LdapBean::LdapBean(){}
LdapBean::~LdapBean(){}
/*=========================================================

=========================================================*/
void LdapBean::filtroInit(cchar *filtro){
  String
    value=getStr(filtro).trim();

  if(!value.len())
    setStr(filtro,"%s",getStr("ldap.query").ptr());
}
/*=========================================================

=========================================================*/
void LdapBean::search(void){
  BEAN(Ldap,ldap,0);
  String
    row=fileLoad("admin/ldap/tableRow.html"),
    exemplo=getStr("exemplo");
  ByteArray
    buffer;
  Map
    out;
  char
    ok=ldap.cmdSearch(exemplo,out);

  if(!ok)
    message(MSG_ERROR,"Alguma coisa sai errada.");
  else{
    Split
      item=out.keys();

    if(!item.len()){
      message(MSG_ALERT,"A pesquisa não retornou nada.");
      goto FIM;
    }
    for(int i=0;i<item.len();i++)
      buffer.writef(
        row,
        item[i].ptr(),
        detalhe(out.get(item[i])).ptr()
      );
  }
  setStr("LdapTable","%s",buffer.toString().ptr());
  FIM:
  dispatch("admin/ldap/ldap.html");
  getPhp().getGcObject().add(&ldap);
}
/*=========================================================

=========================================================*/
String LdapBean::detalhe(MapItem &item){
  Map
    &map=*(Map*)item.asObj();
  Split
    key=map.keys();
  int
    max=0;
  ByteArray
    buffer;

  for(int i=0;i<key.len();i++)
    if(key[i].len()>max)
      max=key[i].len();
  for(int i=0;i<key.len();i++)
    buffer.writef(
      "%-*.*s :%s\n",
      max,max,key[i].ptr(),
      map.get(key[i]).asStr().ptr()
    );
  return buffer.toString();
}
