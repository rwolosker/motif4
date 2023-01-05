/**********************************************************

**********************************************************/
class PhpResource:public WeBean{
  public:
    PhpResource();
    ~PhpResource();
    String find(cchar *file);
    PhpResource &loadStream(cchar *file,ByteArray &buffer);
    PhpResource &load(cchar *file,String &buffer);
    PhpResource &parse(String &buffer);
    PhpResource &send(String &buffer);
};
