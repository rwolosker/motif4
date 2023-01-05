if(document.querySelectorAll==undefined)
  document.prototype.querySelectorAll=function(query){
    var item=query.split(/\./);
    var list=[];
    
    
    document.getElementsByTagName(item[0]).forEach(function(tag){
      if(tag.className&&tag.className.indexOf(item[1])>=0)
        list.push(tag);
    });
    return list;
  };
/*=========================================================
dom.disable_window_open_feature.location;false
=========================================================*/
function motifOpenWindow(url,width,height,center){
  var w;
  
  if(undefined!=url){
    var x=(window.screen.width/2)-(width/2);
    var y=(window.screen.height/2)-(height/2);
  
    w=window.open(
      url,
      "",
      ""
        +"location=0,"
        +"menubar=0,"
        +"toolbar=0,"
        +"menubar=0,"
        +"scrollbars=1,"
        +"left="+x+","
        +"top="+y+","
        +"width="+width+","
        +"height="+height
    );
    
  }
  else
    w=window;
  w.resizeTo(width,height);
  if(center){
    var x=(window.screen.width/2)-(width/2);
    var y=(window.screen.height/2)-(height/2);
    w.moveTo(x,y);
  }
  return w;
}
/*=========================================================
  <a onclick="motifChoice(event);">Tipo</a>
  <input type="text" maxlength="20" size="30" name="nome">
  <div class="option">
    <span>Centro</span>
=========================================================*/
function motifChoice(event){
  var input=null;
  var div=null;
  var hidden=null;
  var select=null;
  var nodes;
  var i;
  
  var hidden=function(){
    div.style.display="none";
    document.body.removeEventListener("click",hidden);
  };
  var select=function(action){
    var span=action.target;
    
    action.stopPropagation();
    if(span.tagName&&span.tagName=="SPAN"){
      input.value=action.target.innerHTML;
      div.removeEventListener("click",select);
      hidden();
    }
  };
  event.stopPropagation();
  nodes=event.target.parentNode.childNodes;
  for(i=0;i<nodes.length;i++){
    var item=nodes[i];
    
    if(item.tagName&&item.tagName=="INPUT")
      input=item;
    if(item.tagName&&item.tagName=="DIV"&&item.className=="option"){
      div=item;
      if(div.style.display=="inline"){
        hidden();
        return;
      }
      div.style.display="inline";
      div.style.left=(input.offsetLeft+2)+"px";
      div.style.width=(input.offsetWidth-6)+"px";
      document.body.addEventListener("click",hidden);
      div.addEventListener("click",select);
      if(div.offsetHeight>window.innerHeight){
        div.style.top="1px";
        div.style.maxHeight=(window.innerHeight-10)+"px";
        div.style.overflowY="scroll";
      }
      if(div.offsetHeight+div.offsetTop>window.outerHeight)
        div.style.top=(window.outerHeight-div.offsetHeight-2)+"px";
    }
  }
}
/*=========================================================
action {msg,action,buttons:"Sim|Nao"} obj{close(),label}
type= error,success,info,alert,action
=========================================================*/
function motifMessage(msg,type,timeout){
  var div=null;
  var icone=null;
  var message=null;
  var fecha=null;
  var obj=null;
  var main=document.querySelector("div.main");
  
  div=document.querySelector("div.message");
  if(div)
    div.remove();
  if(!msg)
    return;
  if(type==undefined)
    type="error";
  obj=div=document.createElement("DIV");
  icone=document.createElement("SPAN");
  message=document.createElement("SPAN");
  
  div.className="message";
  icone.className="icone "+type;
  message.className="message";

  div.appendChild(icone);
  div.appendChild(message);
  main.parentNode.insertBefore(div,main.previousSibling);
  
  fecha=function(){
    try{document.body.removeChild(div);}catch(e){}
  };
  if(timeout!=undefined&&timeout){
    var id=null;
    var hidden=function(){
      fecha();
      window.clearInterval(id);
    };
    id=window.setInterval(hidden,timeout);
  }
  if(type!="action")
    message.innerHTML=msg;
  else{
    var objeto=msg;
    var buttons=objeto.buttons.split(/\|/);
    var i;
    
    message.innerHTML=objeto.msg;
    for(i=0;i<buttons.length;i++){
      var a=document.createElement("a");
      var label=buttons[i];
      
      a.innerHTML=label;
      a.className="button";
      a.addEventListener("click",function(event){
        objeto.action({
          close:fecha,
          label:event.target.innerHTML
        });
      });
      message.appendChild(a);
    }
  }
  {
    var basemenu=document.body.querySelectorAll("div.basemenu");
    
    if(basemenu.length>0){
      basemenu=basemenu[0];
      div.style.top=(basemenu.offsetTop+basemenu.offsetHeight)+"px";
    }
    else{
      var top=window["logo"]?logo.offsetTop+logo.offsetHeight:0;
      
      div.style.top=(top)+"px";
    }
  }
  return obj;
}
/*=========================================================
<p class="join">Label</p>
Label: de cada span do basemenu.
       Pode nao existir, cria na hora.
=========================================================*/
function motifMenuConfig(){
  var basemenu=document.querySelector("div.basemenu");
  var div=document.querySelectorAll("div.menu");
  var i;
  var spanClick=function(action){
    action.stopPropagation();
    {
      var span=action.target;
      var menu=span.menu;
      var hidden=null;
      var ele=false;
      
      for(i=0;i<div.length;i++){
        var esse=div[i];
        var display=esse.style.display;
        
        if(esse==menu&&display=="block")
          ele=true;
        div[i].style.display="none";
      }
      if(!ele){
        menu.style.top=(basemenu.offsetTop+basemenu.offsetHeight)+"px";
        menu.style.display="block";
        menu.style.left=span.offsetLeft+"px";
      }
      hidden=function(){
        menu.style.display="none";
        document.body.removeEventListener("click",hidden);
      };
      document.body.addEventListener("click",hidden);
    }
  };
  
  
  if(basemenu==null||basemenu==undefined)
    return;
  for(i=0;i<div.length;i++){
    var menu=div[i];
    var span=menu.getElementsByTagName("span")[0];
    var join=menu.querySelectorAll(".join");
    var after=menu.querySelectorAll(".after");
    var isSubMenu=true
      &&menu.parentNode.tagName=="DIV"
      &&menu.parentNode.className
      &&(menu.parentNode.className.indexOf("menu")>=0);
    
    if(isSubMenu){
      var pai=span.parentNode;
      
      pai.removeChild(span);
      pai.parentNode.insertBefore(span,pai);
      if(!span.className)
        span.className="";
      span.className+=" submenu";
      span.menu=menu;
      span.addEventListener("click",function(action){
        var span=action.target;
        var menu=span.menu;
        var hidden=null;
        var mesmo=menu.style.display!="none";
        
        action.stopPropagation();
        span.parentNode.querySelectorAll("div.menu").forEach(function(p){
          p.style.display="none";  
        });
        if(mesmo)
          return;
        hidden=function(){
          menu.style.display="none";
          document.body.removeEventListener("click",hidden);
        };
        document.body.addEventListener("click",hidden);
        menu.style.display="block";
        menu.style.top=(span.offsetTop)+"px";
        menu.style.left=(span.parentNode.offsetWidth-1)+"px";
      });
      continue;      
    }
    if(join.length==1){
      var filhos=menu.querySelectorAll("a");
      var label=join[0].innerText;
      var pai=Array
        .from(basemenu.querySelectorAll("span"))
        .filter(function(a){return a.textContent==label;});
      
      if(pai.length==1){
        for(var x=0;x<filhos.length;x++){
          filhos[x].remove();
          pai[0].menu.appendChild(filhos[x]);
        }
      }
      if(pai.length==0){
        var s=document.createElement("span");
        var d=document.createElement("div");
        
        s.menu=d;
        s.innerText=label;
        d.className="menu";
        for(var x=0;x<filhos.length;x++){
          filhos[x].remove();
          s.menu.appendChild(filhos[x]);
        }
        basemenu.appendChild(s);
        s.addEventListener("click",spanClick);
        document.body.appendChild(d);
      }
    }
    if(!span)
      continue;
    span.parentNode.removeChild(span);
    span.menu=menu;
    if(after.length==0)
      basemenu.appendChild(span);
    else{
      var spans=basemenu.getElementsByTagName("SPAN");
      var ii;
      
      for(ii=0;ii<spans.length;ii++){
        var esse=spans[ii];
        
        if(esse.innerText==after[0].innerText){
          basemenu.insertBefore(span,esse.nextSibling);
          break;
        }
      }
    }
    span.addEventListener("click",spanClick);
  }  
}
/*=========================================================

=========================================================*/
function motifTextareaConfig(){
  var item=document.getElementsByTagName("textarea");
  var i;
  
  for(i=0;i<item.length;i++)
    item[i].spellcheck=false;
}
/*=========================================================

=========================================================*/
function motifHelp(){
  var all=document.querySelectorAll("div.help");
  var div=all[0];
  var hidden=null;
  
  hidden=function(action){
    var tag=action.target;
    
    while(tag){
      if(tag==div){
        action.stopPropagation();
        return;
      }
      tag=tag.parentNode;
    }
    div.style.display="none";
    document.body.removeEventListener("click",hidden);
  };
  document.body.addEventListener("click",hidden);
  div.style.display="inline";
  return div;
}
/*=========================================================

=========================================================*/
function motifTagLeft(tag){
  var rec=tag.getBoundingClientRect();
  
  if(rec.x)
    return rec.x+2;
  if(rec.left)
    return rec.left+2;
  return 0;
}
/*=========================================================

=========================================================*/
function motifOffsetLeft(control){
  return control.offsetParent.tagName=="BODY"
    ?control.offsetLeft
    :control.offsetLeft+motifOffsetLeft(control.offsetParent);    
}
/*=========================================================

=========================================================*/
function motifOffsetTop(control){
  return control.offsetParent.tagName=="BODY"
    ?control.offsetTop
    :control.offsetTop+motifOffsetTop(control.offsetParent);    
}
/*=========================================================

=========================================================*/
function motifChoiceAjax(event,url,callback){
  var div=document.createElement("div");
  var input=event.target.parentNode.getElementsByTagName("input")[0];
  var inputAjax=document.createElement("input");
  var divField=document.createElement("div");
  var body=document.createElement("div");
  var span=document.createElement("span");
  var aBusca=document.createElement("a");
  var aFecha=document.createElement("a");
  var hidden=function(){
    div.parentNode.removeChild(div);
  };
  var busca=function(){
    var frm=[];
    
    if(callback)
      callback(inputAjax);
    frm.push({name:"query",value:inputAjax.value});
    motifMessage("Por Favor Aguarde...","alert",0);
    motifAjax(url,frm,function(txt,xml){
      motifMessage("Por Favor Aguarde...","alert",1);
      body.innerHTML=xml.response;
    });
  };
  var select=function(action){
    var span=action.target;

    action.stopPropagation();
    if(span.tagName&&span.tagName=="SPAN"&&span.className==""){
      input.value=span.innerHTML;
      hidden();
    }
  };
  event.stopPropagation();
  div.className="option option-ajax";
  div.style.display="inline";
  div.style.left=(motifOffsetLeft(input)-2)+"px";
  div.style.top=(motifOffsetTop(input)-2)+"px";
  div.style.width=(input.offsetWidth-6)+"px";
  div.style.maxHeight=(window.innerHeight-(input.offsetTop-2)-10)+"px";
  div.style.minHeight="85px";
  div.style.overflowY="scroll";
  span.innerHTML="Busca";
  span.className="span-ajax";
  divField.className="field";
  divField.appendChild(span);
  divField.appendChild(inputAjax);
  div.appendChild(divField);
  //-------------------------------------------------------
  divField=document.createElement("div");
  divField.className="field field-command-ajax";
  aBusca.innerHTML="Buscar";
  aFecha.innerHTML="Fechar";
  divField.appendChild(aBusca);
  divField.appendChild(aFecha);
  div.appendChild(divField);
  div.appendChild(body);
  document.body.appendChild(div);
  //-------------------------------------------------------
  inputAjax.style.width=(div.offsetWidth-span.offsetWidth-inputAjax.offsetLeft)+"px";
  inputAjax.type="text";  
  aFecha.addEventListener("click",hidden);
  aBusca.addEventListener("click",busca);
  div.addEventListener("click",select);
}
/*=========================================================

=========================================================*/
function motifAjax(url,itens,func){
  var boundary="--------------------123456";
  var enter="\r\n";
  var buffer="";
  var xml=new XMLHttpRequest();
  var createField=function(item){
    var src="";
    
    src+=enter;
    src+="--"+boundary+enter;
    src+="Content-Disposition: form-data; name=\""+item.name+"\"";
    if(item.fileName)
      src+=" ;filename=\""+item.fileName+"\"";
    src+=enter;
    if(item.contentType)
      src+=" Content-Type: "+item.contentType+enter;
    src+=enter;
    src+=item.value;
    return src;
  };
  itens.forEach(function(item){
    buffer+=createField(item);
  });
  buffer+=enter+"--"+boundary+"--"+enter;
  
  xml.open("POST",url,true);
  xml.setRequestHeader("Content-Type","multipart/form-data, boundary="+boundary);
  xml.onreadystatechange=function(){
    if(xml.readyState==4)
      func(xml.responseText,xml); 
  };
  xml.send(buffer.substring(2));
}
/*=========================================================

=========================================================*/
function motifRemoveMenu(){
  var div=document.getElementsByClassName("basemenu");
  
  if(div.length>0&&(div=div[0]))
    div.parentNode.removeChild(div);
}
/*=========================================================

=========================================================*/
function motifPanel(div){
  div.style.display="inline";
  div.style.top=(window.outerHeight/2-div.offsetHeight/2)+"px";
  div.style.left=(window.outerWidth/2-div.offsetWidth/2)+"px";
}
/*=========================================================
motifBase64toBlob(xml.response,"application/octet-stream");
https://stackoverflow.com/questions/16245767/creating-a-blob-from-a-base64-string-in-javascript
nomeArquivo:data:application/pdf;base64,JV
o value eh so o base64
=========================================================*/
function motifBase64toBlob(value,contentType){
  var vc=atob(value);
  var len=vc.length;
  var vb=new Array(len);
  var i;
  
  for(i=0;i<len;i++)
    vb[i]=vc.charCodeAt(i);
  return new Blob([new Uint8Array(vb)],{type:contentType});
}
/*=========================================================

=========================================================*/
function motifTextAreaIdent(obj){
  var identa=function(event){
    if(event.keyCode==13){
      var txt=event.target;
      var start=txt.selectionStart;
      var end=txt.selectionEnd;
      var antes=txt.value.substring(0,start);
      var depois=txt.value.substring(end);
      var item=antes.split('');
      var i;
      
      event.preventDefault();
      for(i=item.length-1;i>=0;i--)
        if(item[i]=="\n")
          break;
      if(i==-1){
        txt.value=antes+"\n"+depois;
        txt.setSelectionRange(end+1,end+1);
      }
      else{
        var
          n=i++;
        
        while(item[i]==" ")
          i++;
        n=i-n-1;
        if(!n){
          txt.value=antes+"\n"+depois;
          txt.setSelectionRange(end+1,end+1);
        }
        else{
          txt.value=""
            .concat(antes)
            .concat("\n")
            .concat(new Array(n).fill(" ").join(''))
            .concat(depois);
          txt.setSelectionRange(end+1+n,end+1+n);
          if(txt.selectionEnd>=txt.value.length)
            //BUG CHROME 15/05/2021
            txt.scrollTop=65000;
        }
      }
    }
  };
  obj.addEventListener("keydown",identa);
}
/*=========================================================

=========================================================*/
var motifCheckListSort=function(div){
  window.addEventListener("load",function(){
    var item=[];
    
    div.querySelectorAll("div").forEach(function(esse){
      item.push(esse);
    });
    item.sort(function(a,b){
      var ainput=a.querySelector("input").checked?1:0;
      var binput=b.querySelector("input").checked?1:0;
      var pa=a.querySelector("p");
      var pb=b.querySelector("p");
      var p=pa.innerText.localeCompare(pb.innerText);
      var input=ainput==binput?0:ainput>binput?-1:1;
      
      return input?input:p; 
    });
    div.innerHTML="";
    item.forEach(function(esse){
      div.appendChild(esse);  
    });
  });
};
/*=========================================================

=========================================================*/
var motifInputNumber=function(value){
  if(!value||!value.length)
    return "";
  else{
    var item=value.split("");
    var virgula=0;
    var i;
    var novo=[];
    
    for(i=0;i<item.length;i++){
      if(item[i]=="."&&value.indexOf(",")!=-1)
        continue;
      if((item[i]=="."||item[i]==",")&&virgula)
        continue;
      if(item[i]=="."||item[i]==","){
        virgula=1;
        novo.push(",");
        continue;
      }
      if(item[i].charCodeAt()>=48&&item[i].charCodeAt()<=48+9)
        novo.push(item[i]);
    }
    return novo.join("");
  }
};
/*=========================================================

=========================================================*/
var motifFloatBr=function(control){
  window.addEventListener("load",function(){
    control.value=motifInputNumber(control.value);
    control.addEventListener("input",function(){
      var pos=control.selectionEnd;
      
      control.value=motifInputNumber(control.value);
      if(pos>control.value.length)
        pos=control.value.length;
      control.selectionEnd=control.selectionStart=pos;
    });
    control.form.addEventListener("submit",function(action){
      control.value=control.value.replace(",",".");
    });
  });
};
/*=========================================================
ew Date().toLocaleString("pt-BR").substr(0,10);
=========================================================*/
var motifDateFormat=function(value){
  if(!value||!value instanceof Date)
    return "";
  else{
    var dia=new Number(value.getDate()).toString().padStart(2,0);
    var mes=new Number(value.getMonth()+1).toString().padStart(2,0);
    var ano=new Number(value.getYear()+1900).toString();
    
    return dia
      .concat("/")
      .concat(mes)
      .concat("/")
      .concat(ano);
  }
};
/*=========================================================
para cancelar: event.cancel=true;
addEventListener nao funciona
=========================================================*/
var motifSubmit=function(form){
  var evento=new CustomEvent("submit");
  
  evento.cancel=false;
  form.dispatchEvent(evento);
  if(!evento.cancel)
    form.submit();
};
/*=========================================================

=========================================================*/
function motifTableToCsv(table){
  var csv="";
  
  for(r=0;r<table.rows.length;r++){
    for(c=0;c<table.rows[r].cells.length;c++)
      csv+=table.rows[r].cells[c].innerText+";"
    csv+="\r\n";
  }
  return csv;
};
/*=========================================================

=========================================================*/
function motifUpload(callback){
  var input=document.createElement("input");

  input.type="file";
  input.className="motif-file-upload";
  input.style.width="0px";
  input.style.height="0px";
  input.onchange=function(){
    if(input.files.length>0){
      var reader=new FileReader();
      
      reader.readAsDataURL(input.files[0]);
      reader.onload=function(){
        var obj={};
        
        obj.url=reader.result;
        obj.name=input.files[0].name;
        obj.size=input.files[0].size;
        input.remove();
        callback(obj);
      }
    }
  };
  document.body.appendChild(input);
  input.click();
}
/*=========================================================

=========================================================*/
function motifDownloadBlob(blob,name){
  var a=document.createElement("A");
  var id=null;
  var hidden=function(){
    a.parentNode.removeChild(a);
    window.clearInterval(id);
  };
  a.style.display="none";
  a.innerHTML=a.download=name;
  a.href=window.URL.createObjectURL(blob);
  document.body.appendChild(a);
  a.click();
  id=window.setInterval(hidden,15000);
}
/*=========================================================

=========================================================*/
function motifDownloadValue(value){
  var item=value.split(/\:/);
  var nome=item[0];
  var tipo=item[2].split(/\;/)[0];
  var b64=value.split(/\;base64\,/)[1];

  item="";
  motifDownloadBlob(
    motifBase64toBlob(b64,tipo),
    nome
  );
}
/*=========================================================

=========================================================*/
function motifDateValid(value){
  var dia,mes,ano,item,data;
  
  if(!value||value==null||value==undefined||value==""||typeof value!="string")
    return false;
  if(!value.match(/^\d{2}\/\d{2}\/\d{4}$/))
    return false;
  item=value.match(/^(\d{2})\/(\d{2})\/(\d{4})$/);
  dia=parseInt(item[1]);
  mes=parseInt(item[2]);
  ano=parseInt(item[3]);
  data=new Date(ano,mes-1,dia);
  return motifDateFormat(data)==value;
}
/*=========================================================

=========================================================*/
function motifFindTagReverse(obj,tag){
  if(obj.tagName.toLowerCase()==tag.toLowerCase())
    return obj;
  else
    return motifFindTagReverse(obj.parentElement,tag);
}
/*=========================================================

=========================================================*/
function motifAction(element,type,action){
  element.addEventListener(type,action);
}
/*=========================================================

=========================================================*/
function motifActionTarget(event,tagName,className){
  var ok=true
    &&event.target
    &&event.target.tagName
    &&event.target.tagName.toLowerCase()==tagName.toLowerCase()
    &&(0
      ||(className==undefined)
      ||(true
        &&event.target.className
        &&new RegExp(className,"img").test(event.target.className.toLowerCase())));
  
  return ok?event.target:undefined;
}
/*=========================================================

=========================================================*/
function motifCalendar(event,btn){
  var txt=btn.previousElementSibling;
  var data=txt.value.match(/^(\d{1,2})\/(\d{1,2})\/(\d{1,4})$/);
  var week;
  var dia;
  var mes;
  var ano;
  var meses=["Janeiro","Fevereiro","Março","Abril",
             "Maio","Junho","Julho","Agosto",
             "Setembro","Outubro","Novembro","Dezembro"];
  var diaSemana=["D","S","T","Q","Q","S","S"];
  var div=txt.div;
  var control=null;
  var hidden=function(){
    div.style.display="none";
    document.body.removeEventListener("click",hidden);
  };
  var atualiza=function(){
    var tmp;
    var j;

    tmp=new Date(ano,mes-1,dia);
    tmp.setDate(j=1);
    week=tmp.getDay();
    for(i=0;i<control.dias.length;i++){
      if(i<week){
        control.dias[i].innerHTML="";
        continue; 
      }
      tmp.setDate(j++);
      
      if(tmp.getMonth()==data.getMonth())
        control.dias[i].innerHTML=tmp.getDate();
      else
        control.dias[i].innerHTML="";

      if(tmp.getDate()==data.getDate())
        control.dias[i].style.color="blue";
      else
        control.dias[i].style.color="black";
    }
    control.mes.innerHTML=meses[mes-1];
    control.ano.innerHTML=ano;
  };
  var update=function(){
    ano=data.getYear()+1900;
    mes=data.getMonth()+1;
    dia=data.getDate();
    atualiza();
  };

  if(div==undefined){
    div=document.createElement("div");
    div.innerHTML="<div><a>&lt;&lt;</a><p></p><a>&gt;&gt;</a><a>&lt;&lt;</a><p></p><a>&gt;&gt;</a></div><div><p></p><p></p><p></p><p></p><p></p><p></p><p></p></div><div><p></p><p></p><p></p><p></p><p></p><p></p><p></p></div><div><p></p><p></p><p></p><p></p><p></p><p></p><p></p></div><div><p></p><p></p><p></p><p></p><p></p><p></p><p></p></div><div><p></p><p></p><p></p><p></p><p></p><p></p><p></p></div><div><p></p><p></p><p></p><p></p><p></p><p></p><p></p></div><div><p></p><p></p><p></p><p></p><p></p><p></p><p></p></div>";
    div.className="calendar";
    document.body.append(div);
    txt.div=div;
    div.style.left=motifOffsetLeft(txt)+"px";
    div.style.top=(motifOffsetTop(txt)+txt.offsetHeight)+"px";
  }
  div.style.display="inline-block";
  motifAction(document.body,"click",hidden);
  event.stopPropagation();
  data=data==null
    ?new Date()
    :new Date(parseInt(data[3]),parseInt(data[2]-1),parseInt(data[1]));
  txt.value=data.toLocaleDateString("pt-BR");
  ano=data.getYear()+1900;
  mes=data.getMonth()+1;
  dia=data.getDate();

  control=txt.control;
  if(control==undefined){
    var todosP=div.querySelectorAll("p");
    var todosA=div.querySelectorAll("a");

    control={
      diaSemana:[],
      dias:[],
      navegacao:[],
      mes:null,
      ano:null
    };
    txt.control=control;
    for(i=0;i<todosP.length;i++){
      if(i==0){
        control.mes=todosP[i];
        continue;  
      }
      if(i==1){
        control.ano=todosP[i];
        continue;  
      }
      if(i>=2&&i<=8){
        control.diaSemana.push(todosP[i]);
        continue;
      }
      control.dias.push(todosP[i]);
    }
    for(i=0;i<todosA.length;i++)
      control.navegacao.push(todosA[i]);
  }
  for(i=0;i<7;i++)
    control.diaSemana[i].innerHTML=diaSemana[i];
  atualiza();
  div.onclick=function(event){
    var a=motifActionTarget(event,"a");
    var p=motifActionTarget(event,"p");

    event.stopPropagation();
    if(a){
      switch(control.navegacao.indexOf(a)){
        case 0:
          mes-=1;
          data.setMonth(mes-1);
          update();
          break;  
        case 1:  
          mes+=1;
          data.setMonth(mes-1);
          update();
          break;  
        case 2:  
          ano-=1;
          data.setYear(ano);
          update();
          break;  
        case 3:  
          ano+=1;
          data.setYear(ano);
          update();
          break;  
      }
      return;
    }
    if(p){
      var d=parseInt(p.innerHTML);

      if(d){
        data.setDate(d);  
        update();
        txt.value=data.toLocaleDateString("pt-BR");
        div.style.display="none";
      }
      return;
    }
  };
}
/*=========================================================

=========================================================*/
function motifFloatBrParse(value){
  var p=[];
  
  for(var i=value.length-1;i>=0;i--)
    switch(value.charAt(i)){
      case ",":
        p.push(".");
        break;
      case ".":
        break;
      default:
        p.push(value.charAt(i));
    }
  return new Number(p.reverse().join(""));
}
/*=========================================================

=========================================================*/
function motifPanelMove(panel){
  var title=panel.querySelector("div.title");
  var clientX=null;
  var clientY=null;

  var over=function(event){
    var eX=event.clientX;
    var eY=event.clientY;
    var dX=panel.offsetLeft+(eX-clientX);
    var dY=panel.offsetTop+(eY-clientY);

    clientX=eX;
    clientY=eY;
    panel.style.left=dX+"px";
    panel.style.top=dY+"px";
    
  };

  title.onmousedown=function(event){
    event.stopPropagation();
    clientX=event.clientX;
    clientY=event.clientY;
    title.style.userSelect="none";
    motifAction(document.body,"mousemove",over);
  };
  title.onmouseup=function(event){
    document.body.removeEventListener("mousemove",over);
  };
}
