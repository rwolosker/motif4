<?php
  //print_r($_FILES);
  //exit(0);
  
  
  
  
  include_once("config.php");
  include_once("bind.php");
  $lock=fopen("php.lck","w+");
  
  // .htaccess DirectoryIndex app.php
  if(empty($_SERVER["PATH_INFO"])){
    header("Location: app.php/aplicacao.html");
    exit(0);
  }
  header("Connection: close");
  ini_set("upload_max_filesize","100M");
  ini_set("post_max_size","100M");
  
  try{
    while(!flock($lock,LOCK_EX))
      usleep(100);
    bindExec($MAIN,$param);
    bindResponse($param);
  }
  catch(Exception $e){
    echo ($e->getMessage());
  }
  flock($lock,LOCK_UN);
  fclose($lock);
?>
