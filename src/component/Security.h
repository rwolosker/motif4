/**********************************************************

**********************************************************/
class Security:public PhpSecurity{
  public:
  Security();
  ~Security();
  char access(PhpLogin &login,String &action,String &type);
};
