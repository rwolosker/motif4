/**********************************************************

**********************************************************/
class PhpDao:public PhpBean{
  protected:
    PhpDat
      *phpDat;

    void initPhpDao(void);
    Table &useImpl(cchar *name);
  public:
    PhpDao();
    ~PhpDao();
    String serialize(Table &table);
    Table unserialize(cchar *value);
    virtual TableRef use(void)=0;
    virtual void close(void)=0;
    PhpDat &getDat(void);
};
