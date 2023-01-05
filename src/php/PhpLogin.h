/**********************************************************

**********************************************************/
class PhpLogin:public PhpBean{
  public:
    PhpLogin(){};
    ~PhpLogin(){};
    virtual char fromCookie(void)=0;
    virtual Split  &getGrupo(void)=0;

};
