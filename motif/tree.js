window.addEventListener("load",function(){
  var base=%s;
  var __callback=%s;
  /*=======================================================
  
  =======================================================*/  
  var nodeClick=function(event){
    var item=event.target.item;
    
    if(!item.expand){
      item.querySelectorAll("div.tree-node-item")[0].style.display="inline-block";
      item.expand=true;
    }
    else{
      item.querySelectorAll("div").forEach(function(esse){
        if(esse.className.match(/tree-node-item/))
          esse.style.display="none";
        esse.expand=false;
      });
      item.expand=false;
    }
  };
  /*=======================================================
  
  =======================================================*/  
  base.querySelectorAll("div.tree-node").forEach(function(item,a,b,c,d,e){
    if(item.querySelectorAll("a").length>0){
      var span=item.querySelectorAll("span")[0];
      
      item.className+=" tree-expand";
      item.expand=false;
      span.item=item;
      //span.onclick=undefined;//nodeClick;
    }
    item.querySelectorAll("div.tree-node-item").forEach(function(esse){
      esse.expand=false;
    });
  });
  /*=======================================================
  
  =======================================================*/  
  base.addEventListener("click",function(action){
    var ok;
    
    action.stopPropagation();
    ok=true
      &&action.target
      &&action.target.tagName
      &&action.target.tagName=="SPAN";
    if(ok){
      nodeClick(action);
      return;
    }
    ok=true
      &&action.target
      &&action.target.tagName
      &&action.target.tagName=="A";
    
    if(ok){
      var item=[];
      var obj=action.target;

      while(true){
        obj=obj.parentNode.parentNode;
        item.unshift(obj.querySelector("span").innerText);
        if(obj.parentNode==base)
          break;
      }
      __callback(
        action.target,
        item.join("/")
          .concat("/")
          .concat(action.target.innerText)
      );
      return;
    }
  });
});
