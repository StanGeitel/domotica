<?php

function user_exists($username){
		$username = sanitize($username);
		$query = mysql_querry("SELECT COUNT('user_id') FROM 'user' WHERE 'username' = '$username'");
}
?>