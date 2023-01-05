/**********************************************************

**********************************************************/
class LoginBean:public PhpBean{
  public:
  LoginBean();
  ~LoginBean();
  void login(Map &map);
  void list(Map &map);
};
