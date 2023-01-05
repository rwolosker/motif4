/**********************************************************

**********************************************************/
class PhpValidate:public PhpBean{
  protected:
    int
      count;
    char
      sendError,
      stop;

    void initPhpValidate(void);
    void showError(cchar *format,...);
  public:
    PhpValidate();
    ~PhpValidate();
    PhpValidate &unique(Table &table,Table &reg,cchar *prkey,cchar *field);
    PhpValidate &required(Table &reg,cchar *field);
    PhpValidate &cpf(Table &reg,cchar *field,char required=0);
    PhpValidate &cpfCnpj(Table &reg,cchar *field,char required=0);
    PhpValidate &cnpj(Table &reg,cchar *field,char required=0);
    PhpValidate &matricula(Table &reg,cchar *field,char required=0);
    PhpValidate &setSendError(char sendError);
    PhpValidate &setStop(char stop);
    PhpValidate &date(Table &reg,cchar *field,char required=0);
    PhpValidate &id(Table &table,Table &reg,cchar *field);
    PhpValidate &combo(Table &reg,cchar *field,cchar *key=0);
    PhpValidate &zero(void);
    int          getCount(void);
};
