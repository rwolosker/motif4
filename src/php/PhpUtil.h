/**********************************************************

**********************************************************/
class PhpUtil:public WeBean{
  public:
    PhpUtil();
    ~PhpUtil();
    char   *strNull(cchar *value);
    String  strInput(const String &value,char capitalize=1);
    String  dteInput(const String &value);
    String  numInput(const String &value);
    String  fCpf(const String &value);
    String  format(Table &table,cchar *field);
    String  fdouble(Table &table,cchar *field,int dec=2,char floatBr=0);
    String  fdouble(double value,int dec=2,char floatBr=0);
    char    isHexNumber(const String &value);
    char    checkBox(cchar *name);
    void    checkList(cchar *name,Split &out);
    String  ipRequest(void);
    int     userId(const String &key="sessionUser");
    char    hasRole(const String &value);
    String  userAttr(const String &key);
};
