/**********************************************************

**********************************************************/
class PhpBean:public WeBean{
  protected:
    Php
      *php;
    PhpCgi
      *cgi;
    PhpMessage
      *msg;

    virtual void debug(char debug,cchar *format,...);
    void         debugMap(void);
    void         debugTable(Table &table,char dump=0);
    void         debugTableReg(Table &reg);
    void         dispatch(cchar *fileName);
    void         eval(cchar *format,...);
    String       fileLoad(cchar *fileName);
    PhpCgi      &getCgi(void);
    PhpMessage  &getMsg(void);
    Php         &getPhp(void);
    String       getStr(cchar *name);
    int          getInt(cchar *name);
    void         initPhpBean(void);
    void         inject(Object *obj);
    void         injectAllImpl(void *p);
    void         injectAll(...);
    PhpMessage  &message(int type,char *format,...);
    void         printf(cchar *format,...);
    void         setStr(cchar *name,cchar *format,...);
    void         setObj(cchar *name,Object *obj,char free=0);
  public:
    PhpBean();
    virtual ~PhpBean();
    virtual void configure(Map &map);

};
