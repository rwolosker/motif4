/**********************************************************

**********************************************************/
class UsuarioBean:public PhpBean{
  protected:
  char write(Table &reg,Table &usuario);
  void bind(Table &reg);
  char validate(Table &reg,Table &usuario);
  void toResponse(Table &usuario);
  public:
  UsuarioBean();
  ~UsuarioBean();
  void table(Map &map);
  void novo(Map &map);
  void select(Map &map);
  void save(Map &map);
  char byUsuarioGrupoPapel(TableSort &sort,void *a,void *b);
  void permissaoList(Map &map);
  void senhaDefine(Map &map);
  void accessLog(Map &map);
  void senhaTroca(Map &map);
};
