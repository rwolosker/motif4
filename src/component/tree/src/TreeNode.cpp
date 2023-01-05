/*=========================================================

=========================================================*/
void TreeNode::initTreeNode(void){
  nodeLen=0;
  node=0;
}
/*=========================================================

=========================================================*/
TreeNode::TreeNode(){
  initTreeNode();
}
/*=========================================================

=========================================================*/
TreeNode::~TreeNode(){
  if(node)
    cMemFree(node);
}
/*=========================================================

=========================================================*/
TreeNode *TreeNode::nodeAdd(const String &name){
  MapItem
    &item=map.get(PSTRING(name));

  if(!item.isNull())
    return (TreeNode*)item.asObj();
  else{
    TreeNode
      *pnode=instance();

    map.setValue(pnode->name=name,pnode,1);
    node=(TreeNode**)cMemRealloc(node,sizeof(void*)*(nodeLen+1));
    node[nodeLen]=pnode;
    nodeLen++;
    return pnode;
  }
}
/*=========================================================

=========================================================*/
TreeNode *TreeNode::instance(void){
  TreeNode
    *node=new(cMemAlloc(sizeof(TreeNode)))TreeNode();

  return node;
}
/*=========================================================

=========================================================*/
TreeNode &TreeNode::get(int x){
  return *node[x];
}
TreeNode &TreeNode::get(const String &name){
  return *(TreeNode*)map.get(PSTRING(name)).asObj();
}
/*=========================================================

=========================================================*/
void TreeNode::debug(void){
  debug(0);
}
void TreeNode::debug(int level){
  cPrintf("%*.*s%s\n",level,level,"",name.ptr());
  for(int i=0;i<nodeLen;i++)
    get(i).debug(level+2);
}
