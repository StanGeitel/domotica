<?php
	
	include 'init.php'
	
	if(empty($_POST) === false){
		
		$username = $_POST['username'];
		$password = $_POST['password'];
		
		if(empty($username) === true || empty($password) === true){
			$errors[] = 'You need to enter a username and password';
		} else (user_exists($username) === false){
			$errors[] = 'We can\'t find that username';
		}
		
		
	}


?>