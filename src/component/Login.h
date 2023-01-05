/**********************************************************

**********************************************************/
class Login:public PhpLogin{
  protected:
    String
      msg,
      nome,
      serial;
    Number
      usuarioId;
    Split
      grupo,
      papel;

    char     actived(TableRef usuario);
    void     block(TableRef usuario,char ok);
    char     blocked(TableRef usuario);
    TableRef findUser(const String &user);
    virtual  char password(TableRef usuario,const String &pass);
    void     serialize(TableRef usuario);
  public:
    Login();
    ~Login();
    char    fromCookie(void);
    Split  &getGrupo(void);
    Split  &getPapel(void);
    String  getMessage(void);
    String  getNome(void);
    Number  getUsuarioId(void);
    String  getSerial(void);
    String  grupoStr(void);
    String  papelStr(void);
    char    login(const String &user,const String pass);
};
