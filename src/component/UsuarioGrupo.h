/**********************************************************

**********************************************************/
class UsuarioGrupo:public PhpBean{
  public:
  UsuarioGrupo();
  ~UsuarioGrupo();
  void grupoList(Split &list);
  void grupodoUsuario(int id,Split &list);
  String usuarioNome(int id);
  void save(int id,Split &list);
};
