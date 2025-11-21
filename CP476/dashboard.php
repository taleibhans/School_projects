<?php
session_start();
if (!isset($_SESSION['user'])) {
    header('Location: login.php');
    exit;
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Dashboard</title>
</head>
<body>
    <h2>Welcome, <?= htmlspecialchars($_SESSION['user']) ?></h2>
    <ul>
        <li><a href="search.php">Search Students</a></li>
        <li><a href="grades.php">View Grades</a></li>
        <li><a href="logout.php">Logout</a></li>
    </ul>
</body>
</html>
