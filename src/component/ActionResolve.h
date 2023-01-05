/**********************************************************

**********************************************************/
class ActionResolve:public PhpBean{
  protected:
    void csvLoad(Table &table);
  public:
    ActionResolve();
    ~ActionResolve();
    virtual char resolve(String &action,String &type);
};
