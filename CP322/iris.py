# Import necessary libraries
import pandas as pd  # For data handling
from sklearn.model_selection import train_test_split, cross_val_score  # For splitting data and validation
from sklearn.preprocessing import StandardScaler  # For feature scaling
from sklearn.linear_model import LogisticRegression  # Logistic Regression model
from sklearn.ensemble import RandomForestClassifier, GradientBoostingClassifier  # Ensemble models
from sklearn.metrics import accuracy_score, confusion_matrix  # For evaluation metrics
import matplotlib.pyplot as plt  # For visualizing results

# Step 1: Load the dataset
# Specify the file path for the Iris dataset
iris_data_path = "iris.data"  # Ensure this file is in the same directory or provide the full path
columns = ["sepal_length", "sepal_width", "petal_length", "petal_width", "class"]

# Read the dataset into a pandas DataFrame
iris_df = pd.read_csv(iris_data_path, header=None, names=columns)

# Step 2: Encode the class labels
# Map class names to numerical labels for machine learning algorithms
iris_df['class'] = iris_df['class'].map({
    'Iris-setosa': 0,       # Setosa is mapped to 0
    'Iris-versicolor': 1,   # Versicolor is mapped to 1
    'Iris-virginica': 2     # Virginica is mapped to 2
})

# Step 3: Separate features and target
# X contains the features, y contains the target class
X = iris_df.drop("class", axis=1)  # Features
y = iris_df["class"]  # Target

# Step 4: Split the data into training and testing sets
# 80% of the data is used for training, and 20% for testing
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Step 5: Scale the features
# Standardize features to have zero mean and unit variance
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)  # Fit on training data and transform
X_test_scaled = scaler.transform(X_test)  # Transform test data using the same scaler

# Debugging Step: Check train-test split
print("Training set size:", len(X_train))
print("Test set size:", len(X_test))
print("Training set examples:\n", X_train.head())
print("Test set examples:\n", X_test.head())

# Step 6: Train Logistic Regression
log_reg = LogisticRegression(max_iter=200, random_state=42)  # Initialize the model
log_reg.fit(X_train_scaled, y_train)  # Train the model on the training data
log_reg_accuracy = accuracy_score(y_test, log_reg.predict(X_test_scaled))  # Evaluate accuracy on test data
log_reg_train_accuracy = log_reg.score(X_train_scaled, y_train)  # Evaluate training accuracy

# Step 7: Train Random Forest
rf = RandomForestClassifier(n_estimators=100, random_state=42)  # Initialize the Random Forest model
rf.fit(X_train_scaled, y_train)  # Train the model on the training data
rf_accuracy = accuracy_score(y_test, rf.predict(X_test_scaled))  # Evaluate accuracy on test data
rf_train_accuracy = rf.score(X_train_scaled, y_train)  # Evaluate training accuracy

# Step 8: Train Gradient Boosting
gb = GradientBoostingClassifier(n_estimators=100, learning_rate=0.1, random_state=42)  # Initialize the Gradient Boosting model
gb.fit(X_train_scaled, y_train)  # Train the model on the training data
gb_accuracy = accuracy_score(y_test, gb.predict(X_test_scaled))  # Evaluate accuracy on test data
gb_train_accuracy = gb.score(X_train_scaled, y_train)  # Evaluate training accuracy

# Step 9: Evaluate with Cross-Validation
log_reg_cv = cross_val_score(log_reg, X, y, cv=5).mean()  # Cross-validation for Logistic Regression
rf_cv = cross_val_score(rf, X, y, cv=5).mean()  # Cross-validation for Random Forest
gb_cv = cross_val_score(gb, X, y, cv=5).mean()  # Cross-validation for Gradient Boosting

# Debugging Step: Compare training and testing accuracy
print("\nTraining Accuracy:")
print(f"Logistic Regression: {log_reg_train_accuracy * 100:.2f}%")
print(f"Random Forest: {rf_train_accuracy * 100:.2f}%")
print(f"Gradient Boosting: {gb_train_accuracy * 100:.2f}%")

print("\nTesting Accuracy:")
print(f"Logistic Regression: {log_reg_accuracy * 100:.2f}%")
print(f"Random Forest: {rf_accuracy * 100:.2f}%")
print(f"Gradient Boosting: {gb_accuracy * 100:.2f}%")

print("\nCross-Validation Accuracy:")
print(f"Logistic Regression: {log_reg_cv * 100:.2f}%")
print(f"Random Forest: {rf_cv * 100:.2f}%")
print(f"Gradient Boosting: {gb_cv * 100:.2f}%")

# Step 10: Display confusion matrices
log_reg_conf_matrix = confusion_matrix(y_test, log_reg.predict(X_test_scaled))  # Confusion matrix for Logistic Regression
rf_conf_matrix = confusion_matrix(y_test, rf.predict(X_test_scaled))  # Confusion matrix for Random Forest
gb_conf_matrix = confusion_matrix(y_test, gb.predict(X_test_scaled))  # Confusion matrix for Gradient Boosting

print("\nConfusion Matrices:")
print("Logistic Regression:\n", log_reg_conf_matrix)
print("Random Forest:\n", rf_conf_matrix)
print("Gradient Boosting:\n", gb_conf_matrix)

# Step 11: Visualize model accuracies
results = {
    "Logistic Regression": log_reg_accuracy * 100,
    "Random Forest": rf_accuracy * 100,
    "Gradient Boosting": gb_accuracy * 100
}

plt.figure(figsize=(8, 5))  # Set the figure size
plt.bar(results.keys(), results.values(), color=['blue', 'green', 'orange'], alpha=0.7)  # Create the bar chart
plt.title("Model Accuracy Comparison (Iris Dataset)", fontsize=14)  # Set the title
plt.ylabel("Accuracy (%)", fontsize=12)  # Label the y-axis
plt.xlabel("Model", fontsize=12)  # Label the x-axis
plt.ylim(90, 100)  # Set the y-axis range
plt.grid(axis="y", linestyle="--", alpha=0.6)  # Add a grid for readability
plt.tight_layout()  # Adjust the layout
plt.show()  # Display the chart
