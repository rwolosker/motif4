/**********************************************************

**********************************************************/
class SqLiteDat:public PhpBean{
  protected:
    Gc
      gc;
    SqLite
      sql;
    String
      fileName;
  public:
    SqLiteDat();
    ~SqLiteDat();
    SqLite &getSqLite(void);
    void configure(Map &map);
    SqLiteDat &setFileName(const String &fileName);
    String     getFileName(void);



};
