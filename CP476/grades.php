<?php
session_start();
if (!isset($_SESSION['user'])) {
    header('Location: login.php');
    exit;
}

require 'db_config.php';

// Fetch all students and calculate final grades
$sql = "SELECT N.student_id, N.full_name, C.test_score, C.final_exam_score,
        (C.test_score * 0.4 + C.final_exam_score * 0.6) AS final_grade
        FROM NameTable N JOIN CourseTable C ON N.student_id = C.student_id";
$result = $conn->query($sql);
?>

<!DOCTYPE html>
<html>
<head>
    <title>Student Grades</title>
</head>
<body>
    <h2>Final Grades</h2>
    <table border="1">
        <tr>
            <th>Student ID</th>
            <th>Full Name</th>
            <th>Test Score</th>
            <th>Final Exam Score</th>
            <th>Final Grade</th>
        </tr>
        <?php while ($row = $result->fetch_assoc()): ?>
            <tr>
                <td><?= htmlspecialchars($row['student_id']) ?></td>
                <td><?= htmlspecialchars($row['full_name']) ?></td>
                <td><?= $row['test_score'] ?></td>
                <td><?= $row['final_exam_score'] ?></td>
                <td><?= number_format($row['final_grade'], 2) ?></td>
            </tr>
        <?php endwhile; ?>
    </table>
</body>
</html>
