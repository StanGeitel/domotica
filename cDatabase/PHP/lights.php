
<html>

<body>
	<link rel="stylesheet" type="text/css" href="style.css">
	
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
			
			if ($result = $mysqli->query("SELECT * FROM lights")) {
			
			

			if (isset($_POST['addSet'])){
				$room = $_POST['addSet'];
				
				
				$row_cnt = $result->num_rows;
				$row_cnt++;
			
				$querryString = "";
			
				$querryString = "INSERT INTO lights VALUES ( %d , '%s' ,0 )";
				$querryString = sprintf($querryString, $row_cnt, $room);
				
				//echo $querryString;
				
				if ($mysqli->query($querryString) === TRUE) {
					echo "New record created successfully";
				} else {
					//echo "Error: " . $querryString . "<br>" . $mysqli->error;
				}
				unset($_POST['addSet']);
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
			
			if ($result = $mysqli->query("SELECT * FROM lights")) {
			
			$row_cnt = $result->num_rows;
			
			

			if (isset($_POST['removeSet'])){
				$id = $_POST['removeSet'];
				
				if($id == $row_cnt){
					$querryString = "DELETE FROM lights WHERE id=%d";
					$querryString = sprintf($querryString, $id);
					$mysqli->query($querryString) === TRUE;
					echo "Light has ben removed successfully";
				}
				else if($id > $row_cnt){
					echo "give in an id between 1 and $row_cnt";
					
				}
				else if($id < $row_cnt && $id > 0){
					
					$querryString = "DELETE FROM lights WHERE id=%d";
					$querryString = sprintf($querryString, $id);
					$mysqli->query($querryString) === TRUE;
					
					echo $row_cnt;
					for($i = 0; $i <= $row_cnt;$i++){
						
						if($i > $id && $i <= $row_cnt){
							$querryStringId = "UPDATE lights SET id=%d WHERE id=%d";
							$j = $i -1;
							$querryStringId = sprintf($querryStringId, $j, $i);
							
							//echo $querryStringId;
							
							$mysqli->query($querryStringId) === TRUE;
							
							
						}
						
					}
				echo "Light has ben removed successfully";
				}
								
				
			
				
				
				
				//if ($mysqli->query($querryString) === TRUE && $id > 1 && $id <= $row_cnt) {
				//	echo "Light has ben removed successfully";
				//} else {
					//echo "Error: " . $querryString . "<br>" . $mysqli->error;
				//}
				unset($_POST['removeSet']);
			}
			
			}
			
		}
			
		if (isset($_POST['removeLight'])) {
				  
					removeLight();
				}
		
		?>
	


	
	
	<?php

		
		$query = "SELECT * FROM lights";
		if ($stmt = $mysqli->prepare($query)) {

			/* execute query */
			$stmt->execute();

			/* store result */
			$stmt->store_result();
			
			
			$startValue = 'off';
			
			
			$numRows = $stmt->num_rows;
			echo"<form  action='lights.php' method='POST'>";
			for($i = 1; $i <= $numRows; $i++){
				$query = "SELECT room FROM lights WHERE id = '$i'";
				$result = mysqli_query($mysqli, $query);
				$row = mysqli_fetch_assoc($result);
				
				echo $row['room'];
				
				echo "\t";

				//printf("row: %d .\n", $i);
				$format = 'status%d';
				$status = sprintf($format, $i);
				$querry = "SELECT * FROM lights WHERE id='$i'";
				if ($stmt = $mysqli->prepare($query)) {
					$stmt->execute();
					$stmt->store_result();
					echo "<font size='4' color='black'>lamp $i</font>";
					echo "<label class='switch'>";
					echo "<input type='hidden' name='$status' value='$startValue' >";
					echo "<input type='checkbox' name='$status' value='on' >";
										echo "<span class='slider round'></span>";
					echo"</label> <br/>\n";
				}
				
			}	
		
			

			echo "<input type='submit' name='verzenden' value='Verzend!!'>";	
			echo "</form>";
			
			for($i = 1; $i <= $numRows; $i++){
				$status = sprintf($format, $i);
				if (isset($_POST["$status"])){
					echo $_POST["$status"] . "<br>";
					if($_POST["$status"] == "on"){
						$querry =  "UPDATE lights SET onOff='1' WHERE id=$i";
					}
					else if($_POST["$status"] == "off"){
						$querry =  "UPDATE lights SET onOff='0' WHERE id=$i";
					}
					if ($mysqli->query($querry) === TRUE) {
						
					} else {
					echo "Error updating record: " . $conn->error;
					}

				}
				
			}

			/* close statement */
			$stmt->close();
}


	


	?>
	
	<!--<form name="form" action="" method="post">
	
	</form>-->
	
	<link rel="stylesheet" type="text/css" href="styleIndex.css"/>

	<div class="navbar">
		<a href="index.html">Home</a>
		<a href="lights.php">Lights</a>
		<a href="dimLights.php">DimLights</a>
	</div>
	
		<div class="main">
		<p>Some text some text some text some text..</p>
	</div>

	
	Fill in room name to add light
	<form action="lights.php" method="post">
	<input type="hidden" name="addLight" value="run">
	<input type="text" name="addSet" id="addSet" >
	<input type="submit" value="Add light">
	</form>
	Fill in light id to remove light
	<form action="lights.php" method="post">
	<input type="hidden" name="removeLight" value="run">
	<input type="text" name="removeSet" id="removeSet" >
	<input type="submit" value="Remove light">
	</form>
	

	
</body>
</html>












<!--
	 Rectangular switch
	<label class="switch">
	<input type="checkbox">
	<span class="slider"></span>
	</label>

	Rounded switch
	<label class="switch">
	<input type="checkbox">
	<span class="slider round"></span>
	</label>
//-->