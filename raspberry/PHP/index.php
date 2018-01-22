<?php
session_start();
if(!isset($_SESSION['SESS_ADMINLOGGEDIN'])){
	header("location: login.php");
}
?>
<html>
	<head>
		<title>Welkom om de hoofdpagina</title>
	</head>
	<body>
	
	<link rel="stylesheet" type="text/css" href="styleIndex.css"/>

	<div class="navbar">
		<a href="index.php">Home</a>
		<a href="lights.php">Lights</a>
		<a href="dimLights.php">DimLights</a>
	</div>

	<div class="main">
		<p>Some text some text some text some text..</p>
	</div>
	
	welkom op de webpagina van het KNX domotica systeem!</br>
	<a href="logout.php">log out</a>
	
	<br>
	<br>
	<br>
	<br>
	

		
	</div>
	</body>
</html>