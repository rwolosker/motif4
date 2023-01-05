Security::Security(){};
Security::~Security(){};
char Security::access(PhpLogin &login,String &action,String &type){
  Login
    &plogin=(Login&)login;

  return FOUND(plogin.getPapel().search(&action));
}
