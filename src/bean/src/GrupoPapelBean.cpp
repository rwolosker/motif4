/*=========================================================

=========================================================*/
void GrupoPapelBean::write(int idGrupo,Split &selecionado){
  GrupoPapel
    grupoPapel;

  inject(&grupoPapel);
  grupoPapel.save(idGrupo,selecionado);
}
/*=========================================================

=========================================================*/
void GrupoPapelBean::toResponse(int idGrupo){
  GrupoPapel
    grupoPapel;
  SplitRef
    todos=*Split().instance(),
    dele=*Split().instance();

  inject(&grupoPapel);
  getMap()
    .setValue("papelTodos",&todos,1)
    .setValue("papelGrupo",&dele,1)
    .setValue("grupoId",idGrupo)
    .setValue("grupoNome",grupoPapel.grupoNome(idGrupo));

  grupoPapel.papelList(todos);
  grupoPapel.papeldoGrupo(idGrupo,dele);

}
/*=========================================================

=========================================================*/
GrupoPapelBean::GrupoPapelBean(){}
GrupoPapelBean::~GrupoPapelBean(){}
/*=========================================================

=========================================================*/
void GrupoPapelBean::select(Map &map){
  int
    idGrupo=getMap().get("idGrupo").asInt();

  toResponse(idGrupo);
  dispatch("admin/grupo/grupoPapelEdit.html");
}
/*=========================================================

=========================================================*/
void GrupoPapelBean::save(Map &map){
  int
    idGrupo=getMap().get("grupoId").asInt();
  PhpUtil
    util;
  Split
    selecionado;

  inject(&util);
  util.checkList("item",selecionado);

  write(idGrupo,selecionado);
  message(MSG_SUCCESS,"Realizado com Sucesso.");
  toResponse(idGrupo);
  dispatch("admin/grupo/grupoPapelEdit.html");
}


