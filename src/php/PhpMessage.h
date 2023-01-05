#define MSG_INFO    0
#define MSG_ERROR   1
#define MSG_ALERT   2
#define MSG_SUCCESS 3
/********************************************************************

********************************************************************/
class PhpMessage:public Object,public ExtMap{
  protected:
    int
      time;

    void initPhpMessage(void);
  public:
    PhpMessage();
    ~PhpMessage();
    PhpMessage &setTime(int value);
    PhpMessage &setMessage(int type,cchar *format,...);
};
