/*=========================================================

=========================================================*/
PhpTag::PhpTag(){
}
PhpTag::~PhpTag(){
}
/*=========================================================

=========================================================*/
String PhpTag::echo(cchar *value){
  return value;
}
/*=========================================================

=========================================================*/
String PhpTag::include(cchar *fileName){
  String
    buffer=fileLoad(fileName);

  if(buffer.len()){
    WeParse
      parse;

    inject(&parse);
    parse.parse(buffer);
  }
  return buffer;
}
/*=========================================================

=========================================================*/
String PhpTag::render(int test,cchar *fileName){
  return test?include(fileName):"";
}
/*=========================================================

=========================================================*/
String PhpTag::split(cchar *value,cchar *separator,int pos){
  if(!value||!*value)
    return "";
  else
    return Split().split(value,separator)[pos];
}
/*=========================================================

=========================================================*/
String PhpTag::keep(cchar *name,cchar *value){
  return String().sprintf(
    fileLoad("tag/keep.html").ptr(),
    name,
    value?value:""
  );
}
/*=========================================================

=========================================================*/
String PhpTag::choiceAjax(cchar *label,cchar *name,cchar *value,int size,int maxLen,cchar *url){
  return String().sprintf(
    fileLoad("tag/choiceAjax.html").ptr(),
    url,
    label,
    input(name,value,size,maxLen,0).ptr()
  );
}
/*=========================================================

=========================================================*/
String PhpTag::input(cchar *name,cchar *value,int size,int maxLen,int readonly){
  return String().sprintf(
    fileLoad("tag/input.html"),
    name,
    value?value:"",
    size,
    maxLen?maxLen:size,
    readonly?"readonly":""
  );
}
/*=========================================================

=========================================================*/
String PhpTag::postWait(int formIndex){
  return String().sprintf(
    fileLoad("tag/postWait.html").ptr(),
    formIndex
  );
}
/*=========================================================

=========================================================*/
int PhpTag::fieldLen(cchar *tableName,cchar *fieldName){
  Table
    *table=(Table*)getMap().get(tableName).asObj();

  if(!table){
    BEANMAP(PhpDat,dat,"app.php.dat")
    table=&dat.use(tableName);
  }
  return table
    ?table->getDef().get(fieldName).getSize()-1
    :0;
}
/*=========================================================

=========================================================*/
String PhpTag::checkbox(cchar *name,cchar *label,int value){
  return String().sprintf(
    fileLoad("tag/checkbox.html"),
    label,
    name,
    value?"checked":""
  );
}
/*=========================================================

=========================================================*/
String PhpTag::checklist(cchar *name,Split &values,Split &selecteds,cchar *style,cchar *className){
  String
    html=fileLoad("tag/checklist.html"),
    item=fileLoad("tag/checklistitem.html"),
    res;
  ByteArray
    b0,b1;
  WeParse
    parse;

  for(int i=0;i<values.len();i++){
    int
      x=selecteds.search(values[i].me());

    if(FOUND(x))
      b0.writef(
        item,
        "checked",
        name,
        i,
        values[i].ptr(),
        values[i].ptr());
    else
      b1.writef(
        item,
        "",
        name,
        i,
        values[i].ptr(),
        values[i].ptr()
      );
  }
  res=String().sprintf(
    html,
    className?className:"",
    style?style:"",
    b0.toString().append(b1.toString()).ptr(),
    name,
    values.len()
  );

  inject(&parse);
  parse.parse(res);

  return res;
}
/*=========================================================

=========================================================*/
String PhpTag::mapDebug(void){
  Split
    keys=getMap().keys();
  String
    row="<tr style='vertical-align:top'><td>%s</td><td>%s</td><td style='max-width:400px;white-space:pre-wrap;word-wrap:break-word'>%s</td></tr>",
    table="<table><tr><th>key</th><th>type</th><th style='max-width:400px;white-space:pre-wrap;word-wrap:break-word'>value</th></tr>%s</table>",
    buffer;
  Type
    type;

  for(int i=0;i<keys.len();i++){
    MapItem
      &item=getMap().get(keys[i]);

    buffer.appendf(
      row,
      item.getName().ptr(),
      type.name(item.getType()).ptr(),
      item.asStr().ptr()
    );

  }
  return String().appendf(
    table,
    buffer.ptr()
  );
}
/*=========================================================

=========================================================*/
String PhpTag::choice(cchar *name,cchar *label,cchar *value,Split &options,int size,int maxLen){
  Split
    html=Split(fileLoad("tag/choiceOption.html"),'\n');
  ByteArray
    buffer;

  html.add(fileLoad("tag/choice.html"));
  buffer.writef(
    html[3].ptr(),
    label,
    maxLen,
    size,
    name,
    value&&*value?value:""
  );
  buffer.writeStr(html[0]);
  for(int i=0;i<options.len();i++)
    buffer.writef(
      html[1].ptr(),
      options[i].ptr()
    );
  buffer.writeStr(html[2]);
  buffer.writeStr("\n");
  return buffer.toString();
}
/*=========================================================

=========================================================*/
String PhpTag::upload(PhpUpload *bean,cchar *name,cchar *label){
  String
    html=fileLoad("tag/upload/upload.html"),
    item=fileLoad("tag/upload/item.html"),
    res;
  ByteArray
    row;
  WeParse
    parse;

  if(bean)
    for(int i=0;i<bean->len();i++)
      row.writef(
        item,
        bean->get(i).name.ptr(),
        name,
        i,
        bean->get(i).toUrl().ptr(),
        bean->get(i).binary.len()/1024.
      );
  inject(&parse);
  res=String().sprintf(
    html.ptr(),
    name,
    label,
    row.len()?row.toString().ptr():"",
    name
  );
  return parse.parse(res);
}
/*=========================================================

=========================================================*/
void PhpTag::bean(cchar *clazz,cchar *name){
  String
    cl=clazz,
    bn;

  if(name&&*name)
    bn=name;
  else
    bn
      .append(cl.mid(0,1).lCase())
      .append(cl.mid(1));
  getFactory().getBean(bn,cl);
}
/*=========================================================

=========================================================*/
String PhpTag::help(cchar *file,cchar *id){
  String
    html=fileLoad("tag/help.html");
  WeParse
    parse;

  if(!id||!*id)
    id="ajuda";
  html=html
    .replace("#ajuda#",id)
    .replace("#help.html#",file);
  inject(&parse);
  parse.parse(html);
  return html;
}
/*=========================================================

=========================================================*/
String PhpTag::calendar(cchar *label,cchar *name,cchar *value){
  String
    html=fileLoad("tag/calendar.html");

  return html
    .replace("#value#",value&&*value?value:"")
    .replace("#label#",label)
    .replace("#name#",name);
}
/*=========================================================

=========================================================*/
int PhpTag::hasRole(cchar *value){
  BEAN(PhpUtil,util,"app.php.util")

  return util.hasRole(value);
}
/*=========================================================

=========================================================*/
String PhpTag::iif(int test,cchar *ok,cchar *nok){
  return String(test?ok:nok);
}
