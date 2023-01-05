/**********************************************************

**********************************************************/
class LdapBean:public PhpBean{
  protected:
    String detalhe(MapItem &item);
  public:
    LdapBean();
    ~LdapBean();
    void filtroInit(cchar *filtro);
    void search(void);
};
