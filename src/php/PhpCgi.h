/**********************************************************

**********************************************************/
class PhpCgi:public Object,public ExtMap,public ExtFactory,public ExtReflection{
  protected:
    File
      inFile,
      outFile,
      debugFile,
      evalFile;
    ByteArray
      outBuffer,
      debugBuffer,
      evalBuffer;

    void openFile(void);
    void inDump(void);
    void initPhpCgi(void);
  public:
    PhpCgi();
    ~PhpCgi();
    PhpCgi    &debug(char debug,cchar *format,...);
    PhpCgi    &eval(cchar *format,...);
    PhpCgi    &flush(void);
    ByteArray &getDebugBuffer(void);
    ByteArray &getEvalBuffer(void);
    ByteArray &getOutBuffer(void);
    PhpCgi    &main(void);
    PhpCgi    &printf(cchar *format,...);
};
