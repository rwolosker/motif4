/**********************************************************

**********************************************************/
class PapelBean:public PhpBean{
  protected:
  char write(Table &reg,Table &papel);
  void bind(Table &reg);
  char validate(Table &reg,Table &papel);
  void toResponse(Table &papel);
  public:
  PapelBean();
  ~PapelBean();
  void table(Map &map);
  void novo(Map &map);
  void select(Map &map);
  void save(Map &map);
  void methods(Map &map);
  void import(Map &map);
  void remove(Map &map);
  void configExport(Map &map);
  void configImport(Map &map);
};
