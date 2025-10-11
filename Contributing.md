# Contributing to CHIP-8 Emulator

First off, thanks for taking the time to contribute! 🎉  
This project is built to learn and explore how emulation works at a low level, and every contribution helps improve it.

## 🧩 How to Contribute

### 1. Fork the Repository

Click the **Fork** button on the top right of the repository page and clone your fork locally:

```bash
git clone https://github.com/<your-username>/chip8-emulator.git

cd chip8-emulator
```

### 2. Create a New Branch

Create a feature or bugfix branch:

bash
Copy code
git checkout -b feature/my-new-feature

### 3. Make Your Changes

Make your edits or improvements in the source code (src/ directory).
Keep your code clean, readable, and well-commented where needed.

### 4. Test Your Changes

Before submitting, make sure everything builds and runs properly:

```bash
make

./chip8 path/to/rom.ch8
```

If possible, test a few common ROMs (e.g. Pong, Tetris, Space Invaders) to ensure compatibility.

### 5. Commit and Push

```bash
Copy code
git add .
git commit -m "Add: description of your change"
git push origin feature/my-new-feature
```

### 6. Submit a Pull Request

Open a pull request (PR) to the main branch with a clear title and description of what your change does.

🧠 Code Style
Use consistent indentation and naming conventions.

Avoid unnecessary global variables.

Keep commits focused and descriptive.

Prefer clarity over cleverness.

💡 Ideas for Contribution

- Fix bugs or improve opcode accuracy.

- Improve timing and input handling.

- Add new debugging tools or visualization features.

- Enhance documentation and comments.

- Optimize rendering or performance.

🧪 Testing
If adding new features, include simple test cases or instructions on how to reproduce your changes.
Future plans include adding automated tests for opcode validation.

📜 License
By contributing, you agree that your contributions will be licensed under the MIT License.

🙌 Thanks
Thank you for helping make this emulator better!
Every contribution — big or small — is appreciated.
