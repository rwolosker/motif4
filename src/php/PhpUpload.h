/**********************************************************

**********************************************************/
class PhpUploadFile:public Object{
  public:
  String
    name,
    type;
  ByteArray
    base64,
    binary;
  PhpUploadFile();
  ~PhpUploadFile();
  PhpUploadFile *instance(void);
  String toUrl(void);
};

class PhpUpload:public PhpBean{
  protected:
    GcObject
      gco,nulo;
    int
      length;

    void initPhpUpload(void);
  public:
    PhpUpload();
    ~PhpUpload();
    PhpUpload     &add(cchar *name,ByteArray &buffer);
    PhpUploadFile &get(int x);
    int            len(void);
    PhpUpload     &load(cchar *name);
    PhpUpload     &remove(int x);
    PhpUpload     &reset(void);
};
