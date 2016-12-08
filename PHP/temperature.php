<?php 

$host="yourhost";
$usr="youruser";
$psw="yourpwd";

$db_name="yourdbname";

// Get the data.

$tempI=$_GET['tempI'];
$tempE=$_GET['tempE'];

$umidI=$_GET['umidI'];
$umidE=$_GET['umidE'];


mysql_connect($host,$usr,$psw) or die("Can't connect! ".mysql_error());

mysql_select_db($db_name) or die("Can't connect! ".mysql_error());


$sql = "INSERT INTO temp (tempI,tempE,umidI,umidE)";
$sql.= "VALUES('$tempI','$tempE','$umidI','$umidE')";

if(mysql_query($sql))
{
	echo "Query OK!";
}else{
	echo"Error in query!";
}

mysql_close();
?>