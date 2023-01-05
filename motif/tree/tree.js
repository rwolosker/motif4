function motifTreeView(event,callback){
  var span=motifActionTarget(event,"span");
  var a=motifActionTarget(event,"a");

  event.stopPropagation();
  if(span){
    var isOpen=span.className.match(/open/);
    var isClose=span.className.match(/close/);
    var item=null;

    span.className=span.className
      .replace(isOpen?"open":"close",isOpen?"close":"open");

    if(isClose&&(item=span.parentNode.querySelector(".tree-node-item")))
      item.style.display="block";
    if(isOpen&&(item=span.parentNode.querySelectorAll(".tree-node-item")))
      item.forEach(function(obj){
        obj.style.display="none";
        if((obj=obj.querySelector("span")))
          obj.className=obj.className.replace("open","close");
      });
    return;
  }
  if(a){
    var caminho=[];
    var corre=function(obj){
      var className=function(onj){
        return obj.className?obj.className:"";
      };
      while(!className(obj).match(/tree-base/)){
        if(!className(obj).match(/item/)){
          if(obj.tagName=="A")
            caminho.push(obj.innerText);
          else{
            var span=obj.querySelectorAll("span");
            
            if(span&&span.length)
              caminho.push(span[0].innerText);
          }
        }
        obj=obj.parentNode;
      }
    };
    corre(a);
    caminho=caminho.reverse();
    callback(a,caminho.join("/"));
    return;
  }
}
