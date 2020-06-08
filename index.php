<!DOCTYPE html>
<html xml:lang="en" lang="en">
<head>
    <meta name="Keywords" content="keywords" /><meta name="Description" content="Grand Gallery" />

    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv="refresh" content="25">
    <!--should be set higher for less traffic use DEVELOPMENT MODE-->
    
    <title>Grand Gallery</title>
    
</head>

<body style="background:#968D87">
 <div id="wrapper">

<?php
	//if ( $fileinfo['extension'] == "webm" ) {
	//echo '<video controls style="" preload="none" loop="1" poster="play.png" width="300">';
	//echo '<source src="' . $file . '"' .'type="video/webm";codecs="vp8, vorbis" />'; 
	//echo '</video>';
echo "hi";

$files = array_diff( scandir("./FTP-client/08-06-2020/"), array(".", "..") );

print_r($files); // just for debugging

sort($files);

foreach($files as $item) {

 echo "$item";

}


//$dir = "./FTP-client/08-06-2020/";
//$exclude = array( ".","..","error_log","_notes" );
//if (is_dir($dir)) {
//    $files = scandir($dir);
//    foreach($files as $file){
//        if(!in_array($file,$exclude)){
          //  echo '<li><img src="' . $dir . $file . '" /></li>';
//           echo $file;
 //       }
//    }

//}



//var vid = document.getElementById("video1");
//alert(vid.duration); 
?>

 <video autoplay width="1000" height="740" controls>
  <source src="output0.webm" type="video/webm">
Your browser does not support the video tag.
</video> 


  </div> <!--wrapper-->
  

</body>
</html>
