# C-Project-2nd-sem-
A console-based C++ application to log meals, track daily calories and protein, and manage a custom food database using local file storage.
A lightweight, fast CLI (Command Line Interface) program written in C++ to help users hit their fitness goals by tracking macronutrients. The app generates date-specific log files to keep your daily eating habits organized.

Key Features:

Meal Logging: Search for foods in your database (case-insensitive) and log them based on serving sizes. The app automatically calculates the total calories and protein.

Daily Summaries: Read your daily log to view a formatted table of everything you've eaten today, along with your total caloric and protein intake.

Custom Food Database: Comes with a pre-populated food_database.csv (Chicken, Oats, Paneer, etc.) and allows users to append new foods and their macro profiles directly from the console.

Local File Storage: Fully offline. Uses standard C++ file I/O (ifstream/ofstream) to persist your food database and create daily log files (e.g., log_03-05-2026.txt).
