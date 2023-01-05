/**********************************************************

**********************************************************/
class PhpDat:public Object,public ExtMap{
  protected:
    Map
      tables;
    GcObject
      gcTables;
  public:
    PhpDat();
    ~PhpDat();
    void    close(cchar *name);
    char    open(cchar *name,Table &table);
    Table  &use(cchar *name);
    String  serialize(Table &table);
    Table   unserialize(cchar *value);
    void    listTables(Split &list);
};
