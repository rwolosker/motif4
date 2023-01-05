/**********************************************************

**********************************************************/
class ConfigBackup:public PhpBean{
  protected:
  Table
    log;
  void restoreLog(const String &tag,int total,int update,int insert);
  void unique(Table &table);
  void mergeUsuario(Table &usuario,ByteArray &buffer);
  void mergeGrupo(Table &grupo,ByteArray &buffer);
  void mergeAlias(Table &alias,ByteArray &buffer);
  void mergePapel(Table &papel,ByteArray &buffer);
  void mergePublico(Table &publico,ByteArray &buffer);
  void mergeGrupoPapel(Table &grupo,Table &papel,Table &grupoPapel,ByteArray &buffer);
  void mergeUsuarioGrupo(Table &usuario,Table &grupo,Table &usuarioGrupo,ByteArray &buffer);
  public:
  ConfigBackup();
  ~ConfigBackup();
  String backup(void);
  String restore(const String &path,ByteArray &buffer);
};
