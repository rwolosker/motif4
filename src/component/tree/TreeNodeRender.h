/**********************************************************

**********************************************************/
class TreeNodeRender:public PhpBean{
  protected:
    String
      id,
      callback;
    void
      *createPath;
    virtual String baseRender(TreeNode &node);
    virtual String nodeRender(TreeNode &node);
    virtual String itemRender(TreeNode &node);
    virtual String baseIdResolve(void);
    virtual String baseOnClickResolve(void);
    virtual String baseOnClickFunctionResolve(void);
    void initTreeNodeRender(void);
  public:
    TreeNodeRender();
    ~TreeNodeRender();
    TreeNodeRender &setId(cchar *id);
    TreeNodeRender &setCallback(cchar *callback);
    TreeNodeRender &setCreatePath(void *createPath);
    TreeNodeRender &setCreatePathResolve(cchar *createPath);
    void treeConfigure(cchar *id,cchar *callback,cchar *createPath);
    String treeRender(void);
    virtual String render(TreeNode &node);
};
