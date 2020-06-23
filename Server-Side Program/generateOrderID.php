<?php

$link = mysqli_connect("localhost","root", "", "abc_supermarket");

if (mysqli_connect_error()) {
  die ("There was an error connecting to the database");
}
/*else{
echo "Connection Successful!";
}*/

date_default_timezone_set('Asia/Kolkata');
$date = date("Y-m-d h:i:s");

$insert = "INSERT INTO `orderdetails` (`orderId`, `userMobile`, `date`) VALUES (NULL, NULL, '$date')";
mysqli_query($link, $insert);

$query = "SELECT `orderId` FROM `orderdetails` WHERE `date` = '$date'";

if ($result = mysqli_query($link, $query)) {
  $row = mysqli_fetch_array($result);

  echo "`".$row['orderId']."`";
}
else{
  echo "Error in Server";
}

?>
