/**********************************************************

**********************************************************/
class EditBean:public PhpBean{
  public:
  EditBean();
  ~EditBean();
  void open(Map &map);
  void save(Map &map);
};
