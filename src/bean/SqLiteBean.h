/**********************************************************

**********************************************************/
int cSqliteConnectDat(void *db,void *aux,int argc,char **argv,void **vtab,char **erro);
int cSqliteCreateDat(void *db,void *aux,int argc,char **argv,void **vtab,char **erro);
class SqLiteBean:public PhpBean{
  protected:
    Gc
      gc;
    void listTable(Split &item);
    void moduleDat(SqLite &sql);
  public:
    SqLiteBean();
    ~SqLiteBean();
    void datList(Map &map);
    void listDat(Split &list);
    void datImport(Map &map);
    void datQuery(Map &map);
    void tables(Map &map);
    void catalogo(Map &map);
};
