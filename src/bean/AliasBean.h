/**********************************************************

**********************************************************/
class AliasBean:public PhpBean{
  protected:
  char write(Table &reg,Table &alias);
  void bind(Table &reg);
  char validate(Table &reg,Table &alias);
  void toResponse(Table &alias);
  public:
  AliasBean();
  ~AliasBean();
  void table(Map &map);
  void novo(Map &map);
  void select(Map &map);
  void reuse(Map &map);
  void save(Map &map);
  void import(Map &map);
  void remove(Map &map);
};
