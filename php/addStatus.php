<?php
// doc du lieu tu website gui ve
$dt_status = $_POST["status"];

// ket noi database
include("config.php");

// gui data xuong database
$sql = "update control set status_web='$dt_status'";
mysqli_query($conn, $sql);

// ngat ket noi voi database
mysqli_close($conn);
?>
