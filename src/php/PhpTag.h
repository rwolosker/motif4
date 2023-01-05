/**********************************************************

**********************************************************/
class PhpTag:public PhpBean{
  public:
    PhpTag();
    ~PhpTag();
    String checkbox(cchar *name,cchar *label,int value);
    String checklist(cchar *name,Split &values,Split &selecteds,cchar *style,cchar *className);
    String choice(cchar *name,cchar *label,cchar *value,Split &options,int size,int maxLen=0);
    String choiceAjax(cchar *label,cchar *name,cchar *value,int size,int maxLen,cchar *url);
    String echo(cchar *value);
    int    fieldLen(cchar *tableName,cchar *fieldName);
    String input(cchar *name,cchar *value,int size,int maxLen=0,int readonly=0);
    String include(cchar *fileName);
    String keep(cchar *name,cchar *value);
    String postWait(int formIndex);
    String split(cchar *value,cchar *separator,int pos);
    String render(int test,cchar *fileName);
    String mapDebug(void);
    String upload(PhpUpload *bean,cchar *name,cchar *label);
    String help(cchar *file,cchar *id=0);
    void   bean(cchar *clazz,cchar *name);
    String calendar(cchar *label,cchar *name,cchar *value);
    int    hasRole(cchar *value);
    String iif(int test,cchar *ok,cchar *nok);
};
