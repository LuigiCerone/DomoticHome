<?php
include('session.php');
?>
    <!DOCTYPE html>
    <html>

    <head>
        <title>Homepage</title>
        <link href="style.css" rel="stylesheet" type="text/css"> </head>

    <body>
        <div id="profile"> <b id="welcome">Welcome<i><?php echo $login_session; ?></i></b> <b id="logout"><a href="logout.php">Log Out</a></b> </div>
        <br>
        <center> <a href="queryuno.php"> Temperature </a>
            <br>
            <br> <a href="querydue.php"> Humidity </a> </center>
    </body>

    </html>