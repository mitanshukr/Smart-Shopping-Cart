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

$query="SELECT `order_Id`, `productId`,`productName`,`productPrice` FROM `purchaselist`,`productdata`
WHERE `order_Id`= $orderId and `product_Id` = `productId` and `product_Id`='$productId' LIMIT 1";

if ($result = mysqli_query($link, $query)) {
  $row = mysqli_fetch_array($result);
  if($row['productPrice'])
  echo "`".$row['productPrice']."`".$row['productName']."`";
  else {
    echo "`"."0"."`"."Product not found."."`";
  }
  $delete = "DELETE FROM `purchaselist` WHERE `order_Id`= '$orderId' and `product_Id`= '$productId' LIMIT 1";
  mysqli_query($link, $delete);
}
else{
  echo "Error in Server";
}

?>
