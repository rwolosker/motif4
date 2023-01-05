/**********************************************************

**********************************************************/
class Php:public Object{
  protected:
    Reflection
      *reflection;
    Factory
      factory;
    GcObject
      gco;
    Map
      *map;
    String
      version;

    void actionMain(void);
    void cgiFlush(void);
    void cgiMain(void);
    void factoryConfig(Map &map);
    void initPhp(void);
    void iniConfig(Map &map);
    void loadDat(void);
    void loadTag(void);
    void loadResource(void);
    void reflectionConfig(Map &map);
    void ssl32(void);
  public:
    Php();
    ~Php();
    GcObject    &getGcObject(void);
    Factory     &getFactory(void);
    Map         &getMap(void);
    Reflection  &getReflection(void);
    Php         &configure(Map &map);
    Php         &setVersion(const String &version);
    Php         &main(int argc,char **argv);
    Php         &mapParse(Map &map);
};
