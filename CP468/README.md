## Tic Tac Toe with Gemini AI

A modern and interactive Tic Tac Toe game built using **Pygame**, enhanced with **Google Gemini AI integration** for dynamic gameplay interaction. Developed as part of a final project for CP468.

## 🎮 Features

- Classic 2-player Tic Tac Toe
- Graphical menus and screens
- AI interaction via Google Gemini (e.g., chat or guidance)
- Custom visuals using `.png` image assets and a custom font
- Pygame-based interface for a smooth gaming experience

## 🧠 AI Integration

This project integrates **Google Gemini's Generative AI** to enhance the game experience. You can interact with the model through prompts built into the game (e.g., hint generation, game tips, etc.).

> ⚠️ **IMPORTANT:** The current Gemini API key is **hardcoded** in `tictactoe.py`. This is not secure. It's recommended to store API keys in environment variables or a separate config file.

## 🗂 Directory Structure

```
cp468_finalproj/
├── src/
│   ├── tictactoe.py        # Main game script
│   ├── *.png               # UI and game image assets
│   ├── poppins.extrabold.ttf # Custom font
├── backups/                # (Optional) Backup files
├── .git/                   # Git configuration
├── .project, .pydevproject # Eclipse project files
```

## 🛠 Requirements

- Python 3.x
- [Pygame](https://www.pygame.org/) (`pip install pygame`)
- `google-generativeai` package (`pip install google-generativeai`)

## 🚀 Getting Started

1. **Install dependencies:**

   ```bash
   pip install pygame google-generativeai
   ```

2. **Run the game:**

   ```bash
   cd cp468_finalproj/src
   python tictactoe.py
   ```

3. **(Optional)** Set your own Gemini API key securely:

   ```bash
   export GEMINI_API_KEY="your_api_key"
   ```

   And in the code:
   ```python
   import os
   genai.configure(api_key=os.getenv("GEMINI_API_KEY"))
   ```

## 👥 Contributors

- Group 1 - CP468 Final Project

## 📄 License

MIT License (Add if applicable)

---

Have fun playing Tic Tac Toe with a smart AI twist! ✨
