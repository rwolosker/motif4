/**********************************************************

**********************************************************/
class GrupoBean:public PhpBean{
  protected:
  /*=========================================================

  =========================================================*/
  char write(Table &reg,Table &grupo);
  void bind(Table &reg);
  char validate(Table &reg,Table &grupo);
  void toResponse(Table &grupo);
  public:
  GrupoBean();
  ~GrupoBean();
  void table(Map &map);
  void novo(Map &map);
  void select(Map &map);
  void save(Map &map);
  void remove(Map &map);
};
