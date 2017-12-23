
<html>

<body>

	<link rel="stylesheet" type="text/css" href="styleIndex.css"/>
	<link rel="stylesheet" type="text/css" href="style.css">

	<div class="navbar">
		<a href="index.html">Home</a>
		<a href="lights.php">Lights</a>
		<a href="dimLights.php">DimLights</a>
	</div>
	
		<div class="main">
		<p>Some text some text some text some text..</p>
	</div>
	
		<?php
			$mysqli = new mysqli("localhost", "root", "", "domotica");

			/* check connection */
			if (mysqli_connect_errno()) {
			printf("Connect failed: %s\n", mysqli_connect_error());
			exit();
			}

	?>
	
	
		<?php
		
		
		
		  function AddLight() {
			
			
			$mysqli = new mysqli("localhost", "root", "", "domotica");

			/* check connection */
			if (mysqli_connect_errno()) {
			printf("Connect failed: %s\n", mysqli_connect_error());
			exit();
			}
			
			if ($result = $mysqli->query("SELECT * FROM dimLights")) {
			
			

			if (isset($_POST['addLightSet'])){
				$room = $_POST['addLightSet'];
				$address = $_POST['addLightAddress'];
				
				$row_cnt = $result->num_rows;
				$row_cnt++;
			
				$querryString = "";
			
				$querryString = "INSERT INTO dimLights VALUES ( %d , '%s',INET_ATON(%d) ,0 )";
				$querryString = sprintf($querryString, $row_cnt, $room, $address);
				
				//echo $querryString;
				
				if ($mysqli->query($querryString) === TRUE) {
					echo "New record created successfully";
				} else {
					//echo "Error: " . $querryString . "<br>" . $mysqli->error;
				}
				unset($_POST['addlightSet']);
			}
			
			}

		}
			if (isset($_POST['addLight'])) {
				  
					addLight();
				}
	
	
		?>
		
		
		<?php
		
		function removeLight() {
			$mysqli = new mysqli("localhost", "root", "", "domotica");
			$querryString = "";
			/* check connection */
			if (mysqli_connect_errno()) {
			printf("Connect failed: %s\n", mysqli_connect_error());
			exit();
			}
			
			if ($result = $mysqli->query("SELECT * FROM dimLights")) {
			
			$row_cnt = $result->num_rows;
			
			

			if (isset($_POST['removeLightSet'])){
				$id = $_POST['removeLightSet'];
				$row_cnt = $result->num_rows;
				if($id == $row_cnt){
					$querryString = "DELETE FROM dimLights WHERE id=%d";
					$querryString = sprintf($querryString, $id);
					$mysqli->query($querryString) === TRUE;
					echo "Light has ben removed successfully";
				}
				else if($id > $row_cnt){
					echo "give in an id between 1 and $row_cnt";
					
				}
				else if($id < $row_cnt && $id > 0){
					echo "remove iets";
					$querryString = "DELETE FROM dimLights WHERE id=%d";
					$querryString = sprintf($querryString, $id);
					$mysqli->query($querryString) === TRUE;
					
					echo $row_cnt;
					for($i = 0; $i <= $row_cnt;$i++){
						
						if($i > $id && $i <= $row_cnt){
							$querryStringId = "UPDATE dimLights SET id=%d WHERE id=%d";
							$j = $i -1;
							$querryStringId = sprintf($querryStringId, $j, $i);
							
							//echo $querryStringId;
							
							$mysqli->query($querryStringId) === TRUE;
							
							
						}
						
					}
				echo "Light has ben removed successfully";
				}

				unset($_POST['removeLightSet']);
				
			}
			
			
			
		}
			
		}
		if (isset($_POST['removeLight'])) {
				  
					removeLight();
		}
		
		?>
	


	
	
	<?php

		
		$query = "SELECT * FROM dimLights";
		if ($stmt = $mysqli->prepare($query)) {

			/* execute query */
			$stmt->execute();

			/* store result */
			$stmt->store_result();
			
			
			
			
			$numRows = $stmt->num_rows;
			for($i = 1; $i <= $numRows; $i++){
				$format = 'change%d';
				$status = sprintf($format, $i);
				if(isset($_POST[$status])){
					if($_POST[$status] == "increase") {
						$querry =  "UPDATE dimlights SET intensity = intensity + 10    WHERE id = '$i'";
					} else if ($_POST[$status] == "decrease") {
						$querry =  "UPDATE dimlights SET intensity = intensity - 10    WHERE id = '$i'";
					}
					if ($mysqli->query($querry) === TRUE) {
						
					} else {
					echo "Error updating record: " . $mysqli->error;
					}
					unset($_POST[$status]);
				}
				
				
				$query = "SELECT * FROM dimLights WHERE id = '$i'";
				$result = mysqli_query($mysqli, $query);
				$row = mysqli_fetch_assoc($result);
				
				if($row['intensity'] > 100){
					$query =  "UPDATE dimlights SET intensity = 100    WHERE id = '$i'";
				}
				else if($row['intensity'] < 0){
					$query =  "UPDATE dimlights SET intensity = 0    WHERE id = '$i'";
				}
				if ($mysqli->query($query) === TRUE) {
						
				} else {
					//echo "Error updating record: " . $mysqli->error;
				}
			}
			

			
			for($i = 1; $i <= $numRows; $i++){
				$query = "SELECT * FROM dimLights WHERE id = '$i'";
				$result = mysqli_query($mysqli, $query);
				$row = mysqli_fetch_assoc($result);
				echo "room ";
				echo $row['room'];
				echo " intensity ";
				echo $row['intensity'];
				echo "<br>";
				

				
				$format = 'change%d';
				$status = sprintf($format, $i);

				
				//echo "<form method='post' action='dimLight.php'>";
				echo"<form  action='dimLights.php' method='post'>";
				echo "<input type='submit' id='increase' name='$status' id='$status' value='increase' />";
				echo "<input type='submit' id='decrease' name='$status' id='$status' value='decrease' />";
				echo "</form>";
				



				//printf("row: %d .\n", $i);

			}

			/* close statement */
			$stmt->close();
}




	?>
	
	<!--<form name="form" action="" method="post">
	
	</form>
	
	<link rel="stylesheet" type="text/css" href="styleIndex.css"/>

	<div class="navbar">
		<a href="index.html">Home</a>
		<a href="lights.php">Lights</a>
		<a href="dimLights.php">DimLights</a>
	</div>-->
	
	<?php
	/*
					else if($id < $row_cnt && $id > 0){
					echo "remove iets";
					$querryString = "DELETE FROM dimLights WHERE id=%d";
					$querryString = sprintf($querryString, $id);
					$mysqli->query($querryString) === TRUE;
	*/
	?>
	




	Fill in room name to add light
	<form action="dimLights.php" method="post">
	<input type="hidden" name="addLight" value="run">
	<input type="text" name="addLightSet" id="addLightSet" >
	<input type="text" name="addLightAddress" id="addLightAddress" >
	<input type="submit" value="Add light">
	</form>
	Fill in light id to remove light
	<form action="dimLights.php" method="post">
	<input type="hidden" name="removeLight" value="run">
	<input type="text" name="removeLightSet" id="removeLightSet" >
	<input type="submit" value="Remove light">
	</form>
	
				


	
</body>
</html>