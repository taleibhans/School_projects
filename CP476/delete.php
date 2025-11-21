<?php
session_start();
if (!isset($_SESSION['user'])) {
    header('Location: login.php');
    exit;
}

require 'db_config.php';

// Check if student ID is provided
if (isset($_GET['id'])) {
    $student_id = $_GET['id'];

    // Delete from CourseTable first due to foreign key constraint
    $stmt1 = $conn->prepare("DELETE FROM CourseTable WHERE student_id = ?");
    $stmt1->bind_param("i", $student_id);
    $stmt1->execute();

    // Delete from NameTable
    $stmt2 = $conn->prepare("DELETE FROM NameTable WHERE student_id = ?");
    $stmt2->bind_param("i", $student_id);
    $stmt2->execute();

    echo "<p>Student record deleted successfully.</p>";
} else {
    echo "<p>Student ID not provided.</p>";
}
?>
<a href="search.php">Back to Search</a>
