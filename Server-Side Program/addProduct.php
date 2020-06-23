<?php

$link = mysqli_connect("localhost","root", "", "abc_supermarket");

if (mysqli_connect_error()) {
  die ("There was an error connecting to the database");
}
/*  else{
echo "Connection Successful!";
}*/
$orderId = $_GET['orderId'];
$productId = $_GET['productId'];

$serverRead = "SELECT * FROM productdata WHERE productId = '$productId'";

if ($result = mysqli_query($link, $serverRead)) {
  $row = mysqli_fetch_array($result);
  echo "`".$row['productPrice']."`".$row['productName']."`";

  $query = "INSERT INTO `purchaselist`(`order_Id`, `product_Id`) VALUES ('$orderId', '$productId')";
  mysqli_query($link, $query);
}
else{
  echo "Error in Server";
}

?>
