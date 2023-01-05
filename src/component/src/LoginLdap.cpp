/*=========================================================

=========================================================*/
char LoginLdap::password(TableRef usuario,const String &pass){
  if(!usuario.getInt("ldap"))
    return Login::password(usuario,pass);
  else{
    Ldap
      ldap;
    String
      matricula=usuario.getStr("matricula");
    char
      ok;

    if(!matricula.len()){
      msg="ERRO: Requer [Matricula] LDAP.";
      return 0;
    }
    inject(&ldap);
    ok=ldap.login(matricula,PSTRING(pass));
    if(!ok)
      msg=ldap.getError();

    return ok;
  }
}
LoginLdap::LoginLdap(){}
LoginLdap::~LoginLdap(){}
