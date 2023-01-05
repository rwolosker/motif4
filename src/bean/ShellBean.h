/**********************************************************

**********************************************************/
class ShellBean:public PhpBean{
  public:
  ShellBean();
  ~ShellBean();
  void upload(Map &map);
  void download(Map &map);
  void execute(Map &map);
  void executeAjax(Map &map);
};
