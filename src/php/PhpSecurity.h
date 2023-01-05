/**********************************************************

**********************************************************/
class PhpSecurity:public PhpBean{
  public:
    PhpSecurity(){};
    ~PhpSecurity(){};
    virtual char access(PhpLogin &login,String &action,String &type)=0;
};
