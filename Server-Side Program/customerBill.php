<style>

table, th, td{
  margin-left: auto;
  margin-right: auto;
  padding: 10px 2px 8px 20px;
  text-align: right;
}
th{
  border-bottom: 2px dotted grey;
}
.first_clm{
  padding: 5px 20px 5px 0;
  text-align: left;
}
table tr:last-child{
  font-weight: bold;
  background-color: #e5e5e5;
}
#bill{
  width: 100%;
  margin: 0 auto;
  margin-top: 35px;
  text-align: center;
}

</style>
<?php

$link = mysqli_connect("localhost", "root", "", "abc_supermarket");

if (mysqli_connect_error()) {

  die ("There was an error connecting to the database");
}

$data = $_POST['orderId'];

$query= "select productName, cast(productPrice as decimal(10,2)) Price, count(*) qty,cast((productPrice*count(*)) as decimal (10,2)) Net_Amt
from productdata, purchaselist
where productId = product_Id and order_Id =$data
group by productName, productPrice
union all
select 'Total Sum:',' ',' ',sum(totalSum)
from (
  select productName, productPrice, count(*) quantity,cast((productPrice*count(*)) as decimal (10,2)) totalSum
  from productdata, purchaselist
  where productId = product_Id and order_Id =$data
  group by productName, productPrice)
  t";
  if($result = mysqli_query($link, $query)){

    date_default_timezone_set('Asia/Kolkata');
    $date = date("Y-m-d h:i:s");

    echo'<div id="bill"><h2>ABC Supermarket Pvt. Ltd.</h2>';
    echo'<p>Bill Receipt<br>';
    echo '<strong>Order Id: '.$data.'</strong><br>';
    echo 'Date: '.$date.'</p>';

    echo "<table class=\"table_style\"><tr><th class=\"first_clm\" >Product Name</th><th>Price</th><th>Quantity</th><th>Net_Amount</th></tr>";
    while ($row = mysqli_fetch_array($result)) {
      echo '<tr><td class="first_clm">'.$row["productName"]."</td><td>".$row["Price"]."</td><td>".$row["qty"]."</td><td>".$row["Net_Amt"]."</td><td>";
    }

    echo "</table><h4>Thank you for Shopping with us!</h4>";
    echo '<br><button onclick="window.print()">Print</button>';
    echo '<p><a href="generateBill.php">Go back</a><p></div>';
  }
  else{
    echo "Order Id not found!...";
  }

  ?>
