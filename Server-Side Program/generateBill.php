<!DOCTYPE html>
<html>
<head>
  <title>Generate Bill - ABC SuperMarket</title>
  <style>
  body{
    background-color: grey;
    padding: 0;
    margin: 0;
  }
  #main{
    background-color: white;
    width: 720px;
    height: 100%;
    margin: 0 auto;
    text-align:center;
    border: 2px solid;
    border-radius: 10px;
    padding-bottom: 50px;
    margin-top: 30px;
  }
  #form-div{
    width: 35%;
    margin: 0 auto;
    padding-top: 20px;
  }
  #form{
    text-align: left;
  }
  #head{
    background-color: brown;
    position: relative;
    top:-22px;
    margin-bottom: -20px;
  }
  h1{
    padding:20px 0 0 0;
    color:yellow;

  }
  p{
    font-size:20px;
    padding-bottom: 20px;
    color:grey;
  }
  .field{
    padding: 5px;
    width: 250px;
    font-size: 15px;
  }
  label{
    font-size: 18px;
  }
</style>

<body>
  <div id="main">
    <div id="head">
      <h1>ABC Supermarket Pvt. Ltd.</h1>
      <h2 style="background-color:maroon; color: grey"> Bill Generation Portal </h2>
      <p> Unauthorized Access is illegal.</br>
        <span style="color:red; background-color:white;"><strong>Enter Order Id to generate bill.
        </strong></span></p>
      </div>
      <div id="form-div">
        <form id="form" action="customerBill.php" method="post">
          <label>Order Id : (required)</label><br>
          <input class="field" name="orderId" placeholder='12-digit Order Id.' type='text' pattern ='[0-9]{12}' required><br><br>
          <label>Customer Mobile no. :</label><br>
          <input class="field" name="mobileNo" placeholder='10- digit Mobile No.' type='text' pattern ='[0-9]{10}' autocomplete="off"><br><br>

          <input style="font-size: 15px; margin-top: 20px" name="submit" type='submit' value='Submit'>
        </div>

        <?php
        if(isset($_POST['submit'])){
          // Fetching variables of the form which travels in URL
          $orderId = $_POST['orderId'];
          $phone = $_POST['mobileNo'];
          if($orderId !='')
          {
            //  To redirect form on a particular page
            header("Location:customerBill.php");
            //echo "<script> window.open('');</script>";
          }
          else{
            echo "error";
          }
        }
        ?>
      </form>
    </div>

  </body>
  </html>
