<?php
// Start session to ensure user is logged in
session_start();
if (!isset($_SESSION['user'])) {
    header('Location: login.php');
    exit;
}

// Include database configuration
require 'db_config.php';

// Initialize variable for search results
$results = [];

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $searchTerm = $_POST['student_name'];

    // Use prepared statement to prevent SQL injection
    $stmt = $conn->prepare("SELECT * FROM NameTable WHERE full_name LIKE ?");
    $likeTerm = "%" . $searchTerm . "%";
    $stmt->bind_param("s", $likeTerm);
    $stmt->execute();

    $results = $stmt->get_result()->fetch_all(MYSQLI_ASSOC);
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Search Students</title>
</head>
<body>
    <h2>Search for a Student</h2>
    <!-- Search Form -->
    <form method="POST">
        Student Name: <input name="student_name">
        <button type="submit">Search</button>
    </form>

    <!-- Display Search Results -->
    <?php if (!empty($results)): ?>
        <h3>Results:</h3>
        <table border="1">
            <tr>
                <th>Student ID</th>
                <th>Full Name</th>
                <th>Actions</th>
            </tr>
            <?php foreach ($results as $row): ?>
                <tr>
                    <td><?= htmlspecialchars($row['student_id']) ?></td>
                    <td><?= htmlspecialchars($row['full_name']) ?></td>
                    <td>
                        <a href="update.php?id=<?= $row['student_id'] ?>">Update</a> |
                        <a href="delete.php?id=<?= $row['student_id'] ?>" onclick="return confirm('Are you sure?')">Delete</a>
                    </td>
                </tr>
            <?php endforeach; ?>
        </table>
    <?php endif; ?>
</body>
</html>
