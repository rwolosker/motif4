/**********************************************************

**********************************************************/
class UsuarioGrupoBean:public PhpBean{
  protected:
  void write(int idUsuario,Split &selecionado);
  void toResponse(int idUsuario);
  public:
  UsuarioGrupoBean();
  ~UsuarioGrupoBean();
  void select(Map &map);
  void save(Map &map);
};
