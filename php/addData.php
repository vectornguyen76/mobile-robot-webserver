<?php
// doc du lieu tu website gui ve
$dt_status = $_POST["dt_status"];
$dt_time = $_POST["dt_time"];

if($dt_status == "Forward")
{
    $dt_status = "f";
}
else if ($dt_status =="Backward")
{
    $dt_status ="b";
}
else if ($dt_status =="Turn Right")
{
    $dt_status ="r";
}
else if ($dt_status =="Turn Left")
{
    $dt_status ="l";
}


// ket noi database
include("config.php");

// gui data xuong database
$sql = "update control set status='$dt_status',time=$dt_time, isUpdated=1";
mysqli_query($conn, $sql);

// ngat ket noi voi database
mysqli_close($conn);
?>
