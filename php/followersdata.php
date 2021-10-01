<?php
//setting header to json
header('Content-Type: application/json');

include("config.php");
$sql = "select * from khoang_cach where stt>(select max(stt) from khoang_cach)-10";
  $result = mysqli_query($conn, $sql);

  //$rown=mysqli_fetch_row($result); 
  //printf ("%s (%s)\n",$row["Lastname"],$row["Age"]);
  $data = array();
  foreach ($result as $row) {
  $data[] = $row;
  }
  //Học truy xuất trên sql
  mysqli_close($conn);

print json_encode($data);