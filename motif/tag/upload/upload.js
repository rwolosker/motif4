/*=========================================================

=========================================================*/
var uploadControlAdd=function(name,table){
  motifUpload(function(obj){
    var row=table.insertRow(-1);
    var cell;
    var divNome=document.createElement("div");
    var textarea=document.createElement("textarea");

    cell=row.insertCell(-1);//nome
    divNome.innerHTML=obj.name;
    textarea.value=obj.name.replace(/\:/,"")+":"+obj.url;
    textarea.name=name+(table.rows.length-2).toString();
    cell.append(divNome,textarea);

    cell=row.insertCell(-1);//size
    cell.innerHTML=new Number(obj.size/1024)
      .toLocaleString("en-us",{maximumFractionDigits:1})
      +"Kb";

    row.insertCell(-1).innerHTML="<a>Remover</a>";
    row.insertCell(-1).innerHTML="<a>Download</a>";
  });
};
/*=========================================================

=========================================================*/
var uploadControlDel=function(a,name){
  var tr=motifFindTagReverse(a,"tr");
  var table=motifFindTagReverse(tr,"table");
  var param={
    msg:"Confirma remover <u>"+tr.querySelector("div").innerHTML+"</u>?",
    buttons:"Sim|Nao",
    action:function(obj){
      if(obj.label=="Sim"){
        tr.remove();
        {
          var txt=table.querySelectorAll("textarea");
          var i;

          for(i=0;i<txt.length;i++)
            txt[i].name=name+i;
        }
      }
      obj.close();
    }
  };
  motifMessage(param,"action",15000);
};
/*=========================================================

=========================================================*/
var uploadControlDownload=function(a){
  var value=motifFindTagReverse(a,"tr").querySelector("textarea").value;
  var item=value.split(/\:/);
  var nome=item[0];
  var tipo=item[2].split(/\;/)[0];
  var b64=value.split(/\;base64\,/)[1];

  item="";
  motifDownloadBlob(
    motifBase64toBlob(b64,tipo),
    nome
  );
};
/*=========================================================

=========================================================*/
var uploadControl=function(id){
  var name=id.id;
  var table=id.querySelectorAll("table")[0];

  motifAction(id,"click",function(action){
    var a=motifActionTarget(action,"a");

    if(!a)
      return;
    switch(a.innerText.toLowerCase()){
      case "adicionar":
        uploadControlAdd(name,table);
        break;
      case "remover":
        uploadControlDel(a,name);
        break;
      case "download":
        uploadControlDownload(a);
        break;
    }
  });
};
