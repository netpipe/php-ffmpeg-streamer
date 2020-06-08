<!DOCTYPE html>
<html xml:lang="en" lang="en">
<head>
    <meta name="Keywords" content="keywords" /><meta name="Description" content="Grand Gallery" />

    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv="refresh" content="25">
    <!--should be set higher for less traffic use DEVELOPMENT MODE-->
        <!-- example usage http://localhost/php-ffmpeg-streamer/?user=admin -->

    <title>Grand Gallery</title>
    
</head>

<body style="background:#968D87">
 <div id="wrapper">

<?php

        $user=$_GET["user"];

	//if ( $fileinfo['extension'] == "webm" ) {
	//echo '<video controls style="" preload="none" loop="1" poster="play.png" width="300">';
	//echo '<source src="' . $file . '"' .'type="video/webm";codecs="vp8, vorbis" />'; 
	//echo '</video>';
$dir = "./FTP-client/08-06-2020/";
//$user = "admin";
$dir = "./software/" . $user . "/" . date("d-m-Y");// date("d/m/Y") 
echo $dir;
$files = array_diff( scandir($dir), array(".", "..") );
//print_r($files); // just for debugging
sort($files);
//foreach($files as $item) {
// echo "$item";

//}
//echo next($files) . "<br>";
//echo  "$dir . end($files)";
echo '<video autoplay width="1000" height="740" controls>';
echo '<source src="' . $dir . end($files) . '" type="video/webm">';
echo 'your browser does not support the video tag.';
echo '</video>';
//echo '<source src="' . "./software/" . date("d/m/Y") . $dir . end($files) . '" type="video/webm">';

//var vid = document.getElementById("video1");
//alert(vid.duration); 
?>

  </div> <!--wrapper-->
  

</body>
</html>
