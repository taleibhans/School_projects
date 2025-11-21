<?php
// Database configuration file
$host = "localhost";
$username = "root";
$password = "";  // Use a secure password in production
$database = "student_db";

// Create a connection
$conn = new mysqli($host, $username, $password, $database);

// Check for connection errors
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
?>
