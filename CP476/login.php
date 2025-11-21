<?php
// Start the session to manage user login state
session_start();

// Check if the request method is POST (i.e., login form submitted)
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Get input from the login form
    $username = $_POST['username'];
    $password = $_POST['password'];

    // WARNING: Hardcoded credentials (not secure for production)
    // In production, fetch and verify from a hashed-password database
    if ($username === 'admin' && $password === 'admin123') {
        // Save user session on successful login
        $_SESSION['user'] = $username;
        // Redirect to dashboard
        header('Location: dashboard.php');
        exit;
    } else {
        // Set error message for invalid login
        $error = "Invalid login.";
    }
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Login</title>
</head>
<body>
    <h2>Login</h2>
    <!-- Login Form -->
    <form method="POST">
        Username: <input name="username"><br>
        Password: <input name="password" type="password"><br>
        <button type="submit">Login</button>
    </form>

    <!-- Display error message if login fails -->
    <?php if (!empty($error)) echo "<p style='color:red;'>$error</p>"; ?>
</body>
</html>
