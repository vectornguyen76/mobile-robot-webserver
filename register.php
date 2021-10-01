<?php
// Include config file
include("./php/config.php");
 
// Define variables and initialize with empty values
$username = $password = $confirm_password = "";
$username_err = $password_err = $confirm_password_err = "";
 
// Processing form data when form is submitted
if($_SERVER["REQUEST_METHOD"] == "POST"){
 
    // Validate username
    if(empty(trim($_POST["username"]))){
        $username_err = "Please enter a username.";
    } else{
        $temp_username = trim($_POST["username"]);
        $sql = "SELECT id FROM accounts WHERE user = '$temp_username'";
        $result = mysqli_query($conn,$sql);
        $count = mysqli_num_rows($result);
        if($count == 1) {
            $username_err = "This username is already taken.";
        }else{
            $username = trim($_POST["username"]);
        }
    }
    
    // Validate password
    if(empty(trim($_POST["password"]))){
        $password_err = "Please enter a password.";     
    } elseif(strlen(trim($_POST["password"])) < 6){
        $password_err = "Password must have atleast 6 characters.";
    } else{
        $password = trim($_POST["password"]);
    }
    
    // Validate confirm password
    if(empty(trim($_POST["confirm_password"]))){
        $confirm_password_err = "Please confirm password.";     
    } else{
        $confirm_password = trim($_POST["confirm_password"]);
        if(empty($password_err) && ($password != $confirm_password)){
            $confirm_password_err = "Password did not match.";
        }
    }
    
    // Check input errors before inserting in database
    if(empty($username_err) && empty($password_err) && empty($confirm_password_err)){
        // Create SQL command
        $sql = "insert into accounts(user,pass) values ('$username','$password')";
        mysqli_query($conn, $sql);

        // disconnect from database
        mysqli_close($conn);    
        $confirm_submit = "Create account successfully!";  
    }
   
}
?>


<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
    <title>Sign up - Embedded System</title>
    <link href="./css/register.css" rel="stylesheet" />
  </head>
  <body>
    <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post">
      <h2>Sign Up</h2>
      <p>
        <label for="username" class="floatLabel">Username</label>
        <input id="username" name="username" type="text" class="form-control <?php echo (!empty($username_err)) ? 'is-invalid' : ''; ?>" value="<?php echo $username; ?>">
        <span class="invalid-feedback"><?php echo $username_err; ?></span>
      </p>
      <p>
        <label for="password" class="floatLabel">Password</label>
        <input id="password" name="password" type="password" class="form-control <?php echo (!empty($password_err)) ? 'is-invalid' : ''; ?>" value="<?php echo $password; ?>">
        <span class="invalid-feedback"><?php echo $password_err; ?></span>
        
      </p>
      <p>
        <label for="confirm_password" class="floatLabel"
          >Confirm Password</label
        >
        <input id="confirm_password" name="confirm_password" type="password"  class="form-control <?php echo (!empty($confirm_password_err)) ? 'is-invalid' : ''; ?>" value="<?php echo $confirm_password; ?>">
        <span class="invalid-feedback"><?php echo $confirm_password_err; ?></span>
        
      </p>
      <p>
        <input type="submit" value="Create My Account" id="submit" />
        <span class="invalid-feedback"></span>
      </p>
	  <p class="success" ><?php echo $confirm_submit; ?>.</p>
      <p class="login" >Already have an account? <a href="login.html">Login here</a>.</p>
    </form>
    
  </body>
</html>
