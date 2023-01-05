/*=========================================================

=========================================================*/
var shellUpload=function(btn){
  uploadPanel.style.display="inline";
};
/*=========================================================

=========================================================*/
var shellDownload=function(btn){
  downloadPanel.style.display="inline";
};
/*=========================================================

=========================================================*/
motifAction(window,"load",function(){
  var uploadText=uploadPanel.querySelectorAll("input")[0];
  var uploadFile=uploadPanel.querySelectorAll("input")[1];
  var downloadText=downloadPanel.querySelectorAll("input")[0];
  var upload=undefined;

  var downloadAction=function(){
    if(!(downloadText.value=downloadText.value.trim()).length){
      motifMessage("Informe caminho do Arquivo.","error",15000);
    }
    motifMessage("Aguarde...","alert",0);
    motifAjax(
      "ShellDownload.htm",
      [{name:"file",value:downloadText.value}],
      function(response){
        motifMessage();
        if(response.match(/^ERRO.*/))
          motifMessage(response,"error",15000);
        else
          motifDownloadValue(response);
      }
    );
  };

  var uploadAction=function(){
    var param=[];

    if(!upload){
      motifMessage("Não há nada para fazer.","error",15000);
      return;
    }
    param.push({
      name:"file0",
      value:uploadText.value+":"+upload.url
    });
    motifMessage("Aguarde...","alert",0);
    motifAjax(
      "ShellUpload.htm",
      param,
      function(response){
        motifMessage();
        if(response.match(/^ERRO.*/))
          motifMessage(response,"error",15000);
        else
          motifMessage(response,"success",15000);
      }
    );
  };



  uploadPanel.onclick=function(event){
    var a=motifActionTarget(event,"a");

    if(a)
      switch(a.innerText.toLowerCase()){
        case "fechar":
          uploadPanel.style.display="none";
          break;
        case "upload":
          uploadAction();
          break;
      }
    event.stopPropagation();
  };
  downloadPanel.onclick=function(event){
    var a=motifActionTarget(event,"a");

    if(a)
      switch(a.innerText.toLowerCase()){
        case "fechar":
          downloadPanel.style.display="none";
          break;
        case "download":
          downloadAction();
          break;
      }
    event.stopPropagation();
  };
  uploadFile.onclick=function(event){
    uploadText.value=uploadFile.value;
    motifUpload(function(obj){
      upload=obj;
      uploadText.value=obj.name;
    });
  };
});

