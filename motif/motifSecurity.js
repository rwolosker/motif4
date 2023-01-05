/*=========================================================

=========================================================*/
var motifSecurityImpl=function(sessionGrupo){
  var item=document.querySelectorAll("[data-sec]");
  var i;
  
  for(i=0;i<item.length;i++){
    var
      ok=false;
    
    item[i].attributes["data-sec"].value.split(/,/).forEach(function(grupo){
      ok|=sessionGrupo.indexOf("|"+grupo+"|")!=-1;
    });
    if(!ok)
      item[i].remove();
  }
};
/*=========================================================
motifProtect=function(){
  motifSecurity=true;
  motifSecurityMain();
};
=========================================================*/
var motifSecurityMain=function(){
  var sessionGrupo="#{app.user.grupo}";
  
  if(motifSecurity)
    motifSecurityImpl(sessionGrupo);
};
/*=========================================================

=========================================================*/
var motifSecurityUserName=function(){
  var name="#{tag.split:str,app.user.nome,' ',0}";
  var basemenu=document.body.querySelectorAll("div.basemenu");
  
  if(name.length>0&&basemenu.length>0){
    var span=document.createElement("SPAN");
    
    span.className="userName";
    span.innerHTML="<span>Oi, "+name+"</span><a href='logout.html'>Sair</a>";
    basemenu[0].appendChild(span);
    span.style.right="1%";
  }
};
