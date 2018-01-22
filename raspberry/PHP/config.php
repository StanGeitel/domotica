<?php
$dbhost = "localhost";
$dbuser = "root";
$dbpassword = "domotica";
$dbdatabase = "domotica";
$db = new mysqli($dbhost, $dbuser, $dbpassword, $dbdatabase) or die("Connection failed: " . $conn->connect_error);
?>
