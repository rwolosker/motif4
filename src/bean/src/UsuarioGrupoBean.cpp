/*=========================================================

=========================================================*/
void UsuarioGrupoBean::write(int idUsuario,Split &selecionado){
  UsuarioGrupo
    usuarioGrupo;

  inject(&usuarioGrupo);
  usuarioGrupo.save(idUsuario,selecionado);
}
/*=========================================================

=========================================================*/
void UsuarioGrupoBean::toResponse(int idUsuario){
  UsuarioGrupo
    usuarioGrupo;
  SplitRef
    todos=*Split().instance(),
    dele=*Split().instance();

  inject(&usuarioGrupo);
  getMap()
    .setValue("grupoTodos",&todos,1)
    .setValue("grupoUsuario",&dele,1)
    .setValue("usuarioId",idUsuario)
    .setValue("usuarioNome",usuarioGrupo.usuarioNome(idUsuario));

  usuarioGrupo.grupoList(todos);
  usuarioGrupo.grupodoUsuario(idUsuario,dele);

}
/*=========================================================

=========================================================*/
UsuarioGrupoBean::UsuarioGrupoBean(){}
UsuarioGrupoBean::~UsuarioGrupoBean(){}
/*=========================================================

=========================================================*/
void UsuarioGrupoBean::select(Map &map){
  int
    idUsuario=getMap().get("idUsuario").asInt();

  toResponse(idUsuario);
  dispatch("admin/usuario/usuarioGrupoEdit.html");
}
/*=========================================================

=========================================================*/
void UsuarioGrupoBean::save(Map &map){
  int
    idUsuario=getMap().get("usuarioId").asInt();
  PhpUtil
    util;
  Split
    selecionado;

  inject(&util);
  util.checkList("item",selecionado);

  write(idUsuario,selecionado);
  message(MSG_SUCCESS,"Realizado com Sucesso.");
  toResponse(idUsuario);
  dispatch("admin/usuario/usuarioGrupoEdit.html");
}
