Iris Flower Classification

This project builds a machine learning pipeline to classify iris flowers into three species using the classic Iris dataset. It compares three models:

Logistic Regression
Random Forest
Gradient Boosting

📊 Dataset
Source: UCI Machine Learning Repository
Features:
Sepal Length
Sepal Width
Petal Length
Petal Width
Classes:
Iris-setosa (0)
Iris-versicolor (1)
Iris-virginica (2)

⚙️ Requirements
pip install pandas scikit-learn matplotlib

🚀 How It Works
Load and preprocess data:
Read iris.data and encode class labels.
Split and scale data:
80% training, 20% testing; standardized using StandardScaler.
Train models:
Logistic Regression, Random Forest, and Gradient Boosting classifiers.
Evaluate performance:
Accuracy (train/test), 5-fold cross-validation, and confusion matrices.
Visualize results:
Bar chart comparing test accuracies.

📈 Evaluation Summary
Test Accuracy:
Logistic Regression: ~96.7%
Random Forest: 100%
Gradient Boosting: ~96.7%
Cross-Validation Accuracy:
All models: ~96–97%

A bar chart is generated to visually compare test accuracies.


