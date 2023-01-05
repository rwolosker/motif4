/**********************************************************

**********************************************************/
class PhpAction:public Object,public ExtMap,public ExtFactory,public ExtReflection,public ExtConfigure{
  protected:
    void initPhpAction();
    char isPublico(String &action);
    void resolve(String &action,String &type);
    void sendFile(String &action);
    void execute(String &action);
    void log(cchar *origem,cchar *action);
  public:
    PhpAction();
    ~PhpAction();
    PhpAction &main(void);
};
