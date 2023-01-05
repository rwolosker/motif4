/**********************************************************

**********************************************************/
class LoginLdap:public Login{
  protected:
  virtual char password(TableRef usuario,const String &pass);
  public:
  LoginLdap();
  ~LoginLdap();
};
