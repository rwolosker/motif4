/**********************************************************

**********************************************************/
class DeployBean:public PhpBean{
  protected:
    PhpUpload
      arquivo;
    void packed(File &f,ByteArray &b);
    void unpacked(PhpUploadFile &item,File &flog);
    void datType(void);
  public:
    DeployBean();
    ~DeployBean();
    void make(Map &map);
    void install(Map &map);

};
