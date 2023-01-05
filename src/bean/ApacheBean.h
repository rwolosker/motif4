/**********************************************************

**********************************************************/
class ApacheBean:public PhpBean{
  public:
  ApacheBean();
  ~ApacheBean();
  char byDataDesc(TableSort &sort,void *a,void *b);
  void table(Map &map);
  void select(Map &map);
};
