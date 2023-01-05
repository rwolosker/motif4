<?php
/*=========================================================

=========================================================*/
function scriptPath(){
  $script=$_SERVER["SCRIPT_FILENAME"];
  $name=str_replace("\\","/",$script);
  $item=str_split($name);
  $i=0;
  
  for($i=count($item)-1;$item[$i]!="/";$i--);
  return substr($script,0,$i);
}
/*=========================================================

=========================================================*/
function utf8Is($value){
  return strstr($value,chr(0xC3))?1:0;
}
/*=========================================================

=========================================================*/
function utf8From($value){
  $len=strlen($value);
  $res="";
  $i=0;
  
  for($i=0;$i<$len;$i++)
    if(ord($value[$i])!=0xC3)
      $res.=$value[$i];
    else
    	if(ord($value[$i+1])>=0x80)
        $res.=chr(ord($value[++$i])+0x40);
    	else
    		$res.=$value[$i];
    		
  return $res;
}
/*=========================================================

=========================================================*/
function utf8Convert($value){
  //return $value;
	return utf8Is($value)
    ?utf8From($value)
    :$value;
}
/*=========================================================

=========================================================*/
function bindExec($cmd,&$param){
  global $MAGIC_QUOTES_GPC;

  $foutname=$name=tempnam(sys_get_temp_dir(),"tmp");
  $fout=fopen($name,"wb");
  $finname=$name=tempnam(sys_get_temp_dir(),"tmp");
  $fin=fopen($name,"wb");
  $fdebugname=$name=tempnam(sys_get_temp_dir(),"tmp");
  $fdebug=fopen($name,"wb");
  $fevalname=$name=tempnam(sys_get_temp_dir(),"tmp");
  $feval=fopen($name,"wb");
  $cwd=scriptPath();
  //bindDownload($cwd);
  ini_set("memory_limit","1024M");
  if(true){
    foreach($_REQUEST as $key=>$val){
      if(!strlen(sprintf("%s",$_REQUEST[$key])))
        continue;
      else{
        $value=$MAGIC_QUOTES_GPC?stripslashes($val):$val;
        $value=utf8Convert($value);
        $buffer=pack("i",strlen($key)).$key.pack("i",strlen($value)).$value;
      }
      fwrite($fin,$buffer);
    }
    foreach($_FILES as $name=>$objeto){
      if($objeto["size"]>0){
        $file=$objeto["name"];
        $type=$objeto["type"];
        $value=file_get_contents($objeto["tmp_name"]);
        $value=base64_encode($value);
        $value="$file:data:$type;base64,$value";
        $key=sprintf("${name}File%d",$pos);
        $value=sprintf("%s",$value);
        $buffer=pack("i",strlen($key)).$key.pack("i",strlen($value)).$value;
        fwrite($fin,$buffer);
        $pos+=1;
      }
    }
    foreach($_COOKIE as $key=>$val){
      if(!strlen(sprintf("%s",$_COOKIE[$key])))
        continue;
      else{
        $value=$MAGIC_QUOTES_GPC?stripslashes($val):$val;
        $buffer=pack("i",strlen($key)).$key.pack("i",strlen($value)).$value;
      }
      fwrite($fin,$buffer);
    }
    foreach($_SERVER as $key=>$val){
      if($key=="argv"||$key=="argc")
        continue;
      $buffer=pack("i",strlen($key)).$key.pack("i",strlen($val)).$val;
      fwrite($fin,$buffer);
    }
    $val=file_get_contents("php://input");
    if(strlen($val)){
      $key="phpinput";
      $buffer=pack("i",strlen($key)).$key.pack("i",strlen($val)).$val;
      fwrite($fin,$buffer);
    }
    fflush($fin);
    $out=sprintf(
      "%s in \"%s\" out \"%s\" debug \"%s\" eval \"%s\"",
      $cmd,
      $finname,
      $foutname,
      $fdebugname,
      $fevalname
    );
    
    fclose($fin);
    fclose($fout);
    fclose($fdebug);
    fclose($feval);
    
    $retval=0;
    //printf("<pre>");
    //system("../../core/lib/ld-2.12.so --library-path ../../core/lib --list  ../main");
    //printf("%s\n",$out); 
    $out=system($out,$retval);
    //printf("%d %d\n",$retval,$out==null); 
    
    $fout=fopen($foutname,"rb");
    $feval=fopen($fevalname,"rb");
    $fdebug=fopen($fdebugname,"rb");

    $param["out"]=stream_get_contents($fout);
    $param["out"].=stream_get_contents($fdebug);
    if($retval>0)
      $param["out"].="MEMORY LEAK $retval";
    
    $param["eval"]=stream_get_contents($feval);
    $param["exit"]=0;
    
    fclose($fout);
    fclose($fdebug);
    fclose($feval);
    
    unlink($finname);
    unlink($foutname);
    unlink($fevalname);
    unlink($fdebugname);
  }
}
/*=========================================================

=========================================================*/
function evalInclude($file){
  include($file);
  unlink($file);
}
/*=========================================================

=========================================================*/
function bindResponse($param){
  global $CONTENT_TYPE;
  
  header($CONTENT_TYPE);
  if(!$param["exit"]){
    $out=$param["out"];
    $eval=$param["eval"];
    $i=strpos($out,"\r\n\r\n");
    
    if(strlen($eval))
      eval($eval);
    if($i)
      echo(substr($out,$i+4));
    else
      echo($out);
  }
}
/*=========================================================

=========================================================*/
function bindDownload($dir){
  global $PROXY_HOST;
  global $PROXY_PORT;
  global $BIN_HOST;
  global $BIN_GET;

  if(!is_dir("$dir/bin"))
    mkdir("$dir/bin",0777,true);
  if(!file_exists("$dir/loaded.txt")){
    $zipfile="$dir/bin/bin.zip";
    $f=fopen($zipfile,"wb+");
    ftruncate($f,0);
    $c=fsockopen("$PROXY_HOST",$PROXY_PORT,$errno,$errstr,30);
    $buffer=""
      ."GET $BIN_GET HTTP/1.1\r\n"
      ."Host: $BIN_HOST\r\n"
      ."Connection: Close\r\n\r\n";
    fwrite($c,$buffer);
    fflush($c);
    while(!feof($c))
      fwrite($f,fgets($c,128));
    fclose($c);
    fclose($f);
    {
      $size=filesize($zipfile);
      $f=fopen($zipfile,"rb");
      $buffer=fread($f,$size);
      $pos=strpos($buffer,"\r\n\r\n");
      $buffer=substr($buffer,$pos+4);
      fclose($f);
      $f=fopen($zipfile,"wb+");
      ftruncate($f,0);
      fwrite($f,$buffer);
      fclose($f);
    }
    $zip=zip_open($zipfile);
    while($entry=zip_read($zip)){
      $arquivo=zip_entry_name($entry);
      zip_entry_open($zip,$entry);
      $len=zip_entry_filesize($entry);
      if($len){
        $arquivo=explode("/",$arquivo);
        $arquivo=count($arquivo)==1?$arquivo[0]:$arquivo[1];
        $f=fopen("$dir/bin/$arquivo","wb+");
        fwrite($f,zip_entry_read($entry,$len));
        fclose($f);
        chmod("$dir/bin/$arquivo",0755);
      }
      zip_entry_close($entry);
    }
    zip_close($zip);
    unlink($zipfile);
    $f=fopen("$dir/loaded.txt","w+");
    fwrite($f,"ok");
    fclose($f);
  }
}
?>
