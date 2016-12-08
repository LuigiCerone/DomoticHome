<?php
$connection = mysql_connect("yourhost", "youruser", "yourpwd");
$db = mysql_select_db("yourdb", $connection);

session_start();// Starting Session.
// save data.
$user_check=$_SESSION['login_user'];

// SQL Query.
$ses_sql=mysql_query("select username from login where username='$user_check'", $connection);
$row = mysql_fetch_assoc($ses_sql);
$login_session =$row['username'];

if(!isset($login_session)){
    mysql_close($connection);
    header('Location: index.php');
}
?>