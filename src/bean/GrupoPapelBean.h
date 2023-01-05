/**********************************************************

**********************************************************/
class GrupoPapelBean:public PhpBean{
  protected:
  void write(int idGrupo,Split &selecionado);
  void toResponse(int idGrupo);
  public:
  GrupoPapelBean();
  ~GrupoPapelBean();
  void select(Map &map);
  void save(Map &map);
};


