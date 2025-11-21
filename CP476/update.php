<?php
session_start();
if (!isset($_SESSION['user'])) {
    header('Location: login.php');
    exit;
}

require 'db_config.php';

// Fetch student record based on ID
if (isset($_GET['id'])) {
    $student_id = $_GET['id'];
    $stmt = $conn->prepare("SELECT * FROM NameTable NATURAL JOIN CourseTable WHERE student_id = ?");
    $stmt->bind_param("i", $student_id);
    $stmt->execute();
    $student = $stmt->get_result()->fetch_assoc();
}

// Handle form submission to update student data
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $student_id = $_POST['student_id'];
    $full_name = $_POST['full_name'];
    $test_score = $_POST['test_score'];
    $final_exam_score = $_POST['final_exam_score'];

    // Update NameTable
    $stmt1 = $conn->prepare("UPDATE NameTable SET full_name = ? WHERE student_id = ?");
    $stmt1->bind_param("si", $full_name, $student_id);
    $stmt1->execute();

    // Update CourseTable
    $stmt2 = $conn->prepare("UPDATE CourseTable SET test_score = ?, final_exam_score = ? WHERE student_id = ?");
    $stmt2->bind_param("iii", $test_score, $final_exam_score, $student_id);
    $stmt2->execute();

    echo "<p>Student record updated successfully.</p>";
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Update Student</title>
</head>
<body>
    <h2>Update Student Record</h2>
    <?php if (!empty($student)): ?>
    <form method="POST">
        <input type="hidden" name="student_id" value="<?= $student['student_id'] ?>">
        Full Name: <input name="full_name" value="<?= htmlspecialchars($student['full_name']) ?>"><br>
        Test Score: <input name="test_score" type="number" value="<?= $student['test_score'] ?>"><br>
        Final Exam Score: <input name="final_exam_score" type="number" value="<?= $student['final_exam_score'] ?>"><br>
        <button type="submit">Update</button>
    </form>
    <?php else: ?>
        <p>No student found.</p>
    <?php endif; ?>
</body>
</html>
