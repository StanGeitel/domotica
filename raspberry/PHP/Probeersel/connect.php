
<?php
	
	 include 'config.inc.php';
	
	$conn = new mysqli($address, $username, $password, $database_name);
	if ($conn->connect_errno != 0) //er gaat iets fout ...
	{
	die("Probleem bij het leggen van connectie of selecteren van database");
	}

?>
