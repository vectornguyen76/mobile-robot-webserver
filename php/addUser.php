<?php
// doc du lieu tu website gui ve
$acc = $_POST["username"];
$pass = $_POST["password"];

// ket noi database
include("config.php");

// gui data xuong database
$sql = "insert into accounts (user,pass) values ('$acc','$pass')";
mysqli_query($conn, $sql);

// ngat ket noi voi database
mysqli_close($conn);
?>