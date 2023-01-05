/**********************************************************

**********************************************************/
class PermissaoList:public Object,public ExtMap{
  public:
  PermissaoList();
  ~PermissaoList();
  char byUsuarioGrupoPapel(Sort &sort,void *a,void *b);
  void list(Table &table);
  PermissaoList &gruposDoUsuario(int id,Split &list);
  PermissaoList &papelsDoGrupo(Split &grupoList,Split &list);
};
