/**********************************************************

**********************************************************/
class PhpAudit:public WeBean{
  protected:
    void inverte(ByteArray &buffer);
  public:
    PhpAudit();
    ~PhpAudit();
    PhpAudit *instance(void);
    PhpAudit &log(Table &table,cchar *id,cchar *label);
    PhpAudit &toMap(cchar *name="audit");
    String    audit(cchar *panelId,cchar *dataSec,int id,cchar *label);
    void      table(Map &map);
};
