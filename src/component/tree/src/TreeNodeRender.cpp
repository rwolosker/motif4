/*=========================================================

=========================================================*/
TreeNodeRender::TreeNodeRender(){
  initTreeNodeRender();
}
TreeNodeRender::~TreeNodeRender(){
}
/*=========================================================

=========================================================*/
void TreeNodeRender::initTreeNodeRender(void){
  createPath=0;
}
/*=========================================================

=========================================================*/
TreeNodeRender &TreeNodeRender::setId(cchar *id){
  this->id=id;
  return *this;
}
TreeNodeRender &TreeNodeRender::setCallback(cchar *callback){
  this->callback=callback;
  return *this;
}
TreeNodeRender &TreeNodeRender::setCreatePath(void *createPath){
  this->createPath=createPath;
  return *this;
}
TreeNodeRender &TreeNodeRender::setCreatePathResolve(cchar *createPath){
  void
    *pf=getFactory().getMethod(createPath);

  return setCreatePath(pf?pf:0);
}
/*=========================================================

=========================================================*/
void TreeNodeRender::treeConfigure(cchar *id,cchar *callback,cchar *createPath){
  setId(id);
  setCallback(callback);
  setCreatePathResolve(createPath);
}
/*=========================================================

=========================================================*/
String TreeNodeRender::render(TreeNode &node){
  return !node.name.len()
    ?baseRender(node)
    :nodeRender(node);
}
/*=========================================================

=========================================================*/
String TreeNodeRender::itemRender(TreeNode &node){
  return !node.nodeLen
    ?String().sprintf("<a>%s</a>\n",node.name.ptr())
    :render(node);
}
/*=========================================================

=========================================================*/
String TreeNodeRender::nodeRender(TreeNode &node){
  ByteArray
    buffer;

  buffer.writeStr("<div class=\"tree-node\">\n");
  if(!node.nodeLen)
    buffer.writeStr(itemRender(node));
  else{
    buffer.writef("<span class=\"tree-node-close\">%s</span>\n",node.name.ptr());
    buffer.writeStr("<div class=\"tree-node-item\" style=\"display:none\">\n");
    for(int i=0;i<node.nodeLen;i++)
      buffer.writeStr(itemRender(node.get(i)));
    buffer.writeStr("</div>\n");
  }
  buffer.writeStr("</div>\n");
  return buffer.toString();
}
/*=========================================================

=========================================================*/
String TreeNodeRender::baseRender(TreeNode &node){
  ByteArray
    buffer;

  buffer.writef(
    "<div id=\"%s\" class=\"tree-base\" onclick=\"%s\">\n",
    baseIdResolve().ptr(),
    baseOnClickResolve().ptr()
  );
  for(int i=0;i<node.nodeLen;i++)
    buffer.writeStr(render(node.get(i)));
  buffer.writeStr("</div>\n");
  return buffer.toString();
}
/*=========================================================

=========================================================*/
String TreeNodeRender::baseIdResolve(void){
  return id;
}
/*=========================================================

=========================================================*/
String TreeNodeRender::baseOnClickResolve(void){
  return String().sprintf("motifTreeView(event,%s);",baseOnClickFunctionResolve().ptr());
}
/*=========================================================

=========================================================*/
String TreeNodeRender::baseOnClickFunctionResolve(void){
  return callback;
}
/*=========================================================

=========================================================*/
String TreeNodeRender::treeRender(void){
  char
    (*pCreatePath)(Map&,ByteArray&)=0;
  ByteArray
    buffer;
  String
    linha,
    js=fileLoad("tree/tree.js"),
    css=fileLoad("tree/tree.css");
  TreeNode
    root;
  Split
    caminho;

  *(void**)&pCreatePath=createPath;
  pCreatePath(getMap(),buffer);
  buffer.seek(0);
  while(buffer.readLine(linha)){
    char
      *p=linha.ptr();

    if(!*cStrTrim(p,p))
      continue;
    if(*p=='/')
      p+=1;
    caminho.add(p);
  }
  caminho.sort();
  for(int i=0;i<caminho.len();i++){
    Split
      item=Split(caminho[i],'/');
    TreeNode
      *p=&root;

    for(int i=0;i<item.len();i++)
      p=p->nodeAdd(item[i]);
  }

  buffer.reset();
  buffer.writef(
    "\n<style>\n%s\n</style>\n<script>\n%s\n</script>\n%s",
    css.ptr(),
    js.ptr(),
    render(root).ptr()
  );

  return buffer.toString();
}
