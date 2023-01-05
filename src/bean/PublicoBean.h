/**********************************************************

**********************************************************/
class PublicoBean:public PhpBean{
  protected:
  char write(Table &reg,Table &publico);
  void bind(Table &reg);
  char validate(Table &reg,Table &publico);
  void toResponse(Table &publico);
  public:
  PublicoBean();
  ~PublicoBean();
  void table(Map &map);
  void novo(Map &map);
  void select(Map &map);
  void save(Map &map);
  void remove(Map &map);
};
