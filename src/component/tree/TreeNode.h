/**********************************************************

**********************************************************/
class TreeNode:public Object{
  protected:
    Map
      map;
    void initTreeNode(void);
    void debug(int level);

  public:
    TreeNode();
    ~TreeNode();
    TreeNode *nodeAdd(const String &name);
    TreeNode &get(int x);
    TreeNode &get(const String &name);
    TreeNode *instance(void);
    void debug(void);
    String
      name;
    TreeNode
      **node;
    int
      nodeLen;
};
