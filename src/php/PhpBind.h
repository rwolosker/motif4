/**********************************************************

**********************************************************/
#define BTRIMONLY     0x01
#define BNOCAPITALIZE 0x02
#define BFLOATBR      0x04
#define BCAPITALIZE   0x08
#define BINPUT        0x10
#define BNOINPUT      0x20
#define BINTEGER      0x40
#define BBOOLEAN      0x80
class PhpUtil;
class PhpBind:public Object,public ExtMap,public ExtFactory{
  protected:
    PhpUtil
      *util;

    void bindBoolean(MapItem &map,Field &field,int flag);
    void bindDate(MapItem &map,Field &field,int flag);
    void bindNumber(MapItem &map,Field &field,int flag);
    void bindText(MapItem &map,Field &field,int flag);
    void initPhpBind(void);
    String valueInteger(const String &value);
    PhpUtil &getUtil(void);
  public:
    PhpBind();
    ~PhpBind();
    PhpBind &bind(cchar *key,Table &table,cchar *field=0,int flag=0);
    PhpBind &bind(cchar *key,Object *dst,int flag=0);
    PhpBind &bind(cchar *key,Table &table,cchar *field,Table &src,cchar *fieldSrc);
};
