<?php
	session_start();
	$error='';
	if (isset($_POST['submit'])) {
        if (empty($_POST['username']) || empty($_POST['password'])) {
	    $error = "Username or Password are wrong!";
	}
	else
	{
	   // Get $username and $password.
	   $username=$_POST['username'];
	   $password=$_POST['password'];

	   $connection = mysql_connect("yourhost", "youruser", "yourpwd");
	    
	   // For SQL Injection.
	   $username = stripslashes($username);
	   $password = stripslashes($password);
	   $username = mysql_real_escape_string($username);
	   $password = mysql_real_escape_string($password); 
	   $db = mysql_select_db("yourdb", $connection);
	   // SQL query.
	   $query = mysql_query("select * from login where password='$password' AND username='$username'", $connection);
	   $rows = mysql_num_rows($query);
	   if ($rows == 1) {
           $_SESSION['login_user']=$username; // Start session.
	       header("location: profile.php"); // Redirect.
	   } else {
            $error = "Username or Password are wrong!";
            }
	   mysql_close($connection); // Close connection.
	   } //else-close.
	}//if-close.
?>