<?php
session_start();
if(isset($_SESSION['SESS_ADMINLOGGEDIN'])) {
header("Location: index.php");
}
include("config.php");

if(isset($_POST['submit']))
{
	$loginsql = "SELECT * FROM users WHERE username = '" . $_POST['userBox'] . "' AND password = '" . $_POST['passBox']. "'";
	echo $loginsql;
	$loginres = $db->query($loginsql);
	$numrows = mysqli_num_rows($loginres);
	echo $numrows;
	if($numrows == 1)
	{
		$loginrow = mysqli_fetch_assoc($loginres);
		$_SESSION['SESS_ADMINLOGGEDIN'] = 1;
		header("Location: index.php");
		//header("Location: " . $config_basedir . "adminorders.php");
	}
	else
	{
		echo "no rows are found";
		//header("Location: login.php?error=1");
	}
}
else
{
	echo "<h1>Admin Login</h1>";
	if(@$_GET['error'] == 1) {
		echo "<strong>Incorrect username/password!</strong>";
	}
?>
<p>
<form action="<?php echo $_SERVER['SCRIPT_NAME']; ?>" method="POST">
<table>
<tr>
<td>Username</td>
<td><input type="textbox" name="userBox">
</tr>
<tr>
<td>Password</td>
<td><input type="password" name="passBox">
</tr>
<tr>
<td></td>
<td><input type="submit" name="submit" value="Log in">
</tr>
</table>
</form>
<?php

}
?>
