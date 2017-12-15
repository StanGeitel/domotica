<html>
	<head>
		<title>Login systeem</title>
	</head>
	<body>
	

	
	
		<?php
				$connection = mysqli_connect("127.0.0.1", "root", "", "domotica");

			if (!$connection) {
			echo "Error: Unable to connect to MySQL." . PHP_EOL;
			echo "Debugging errno: " . mysqli_connect_errno() . PHP_EOL;
			echo "Debugging error: " . mysqli_connect_error() . PHP_EOL;
			exit;
	}

	echo "Success: A proper connection to MySQL was made! The my_db database is great." . PHP_EOL;
		?>
	
	<?php
		if(@$_POST['login']){
			echo "hoi";
			$user = $_POST['username'];
			$pass = $_POST['password'];

			
			if($user && $pass){
					$querry = "SELECT * FROM users WHERE username='$user'";
					$result = mysqli_query($connection, $querry);
			
				if($row = mysqli_fetch_assoc($result)){
					
					$db_user = $row['username'];
					$db_pass = $row['password'];
					$db_isAdmin = $row['isAdmin'];
				
					if($user == $db_user && $pass == $db_pass){
						if($db_isAdmin == 1){
							echo '<p><b>welcome administrator</b></p>';
							header('Location: index.php');
						}
						else{
							echo '<p><b>You are loged in</b></p>';
							header('Location: index.php');
						}
						
					}
					else{
						echo '<p>Username and Password don\'t match';
					}
				}
				else{
					echo'<p>Username is not in database</p>';
				}
			
			}	
			else{
				echo'<p>Not all fields were filled in</p>';
			}
		}
		?>
		
			<form action="" method="post">
				<table>
					<tr>
						<td>Username</td><td>:</td><td><input type="text" name="username"/></td>
					</tr>
					<tr>
						<td>Password</td><td>:</td><td><input type="password" name="password"/></td>
					</tr>
					<tr>
					
					<input type='submit' name='verzenden' value='Verzend!!'>
					
					</tr>
				
				</table>
			</form>
			

		</body>
	</html>
	