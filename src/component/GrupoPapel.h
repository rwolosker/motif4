/**********************************************************

**********************************************************/
class GrupoPapel:public PhpBean{
  public:
    GrupoPapel();
    ~GrupoPapel();
    void papelList(Split &list);
    void papeldoGrupo(int id,Split &list);
    String grupoNome(int id);
    void save(int id,Split &list);
};
