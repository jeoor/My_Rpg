# MyRPG - A C++ RPG Game with EasyX

![Game Screenshot](./screenshot/screenshot.png)

## 📖 Description

MyRPG is a simple 2D role-playing game developed from scratch using C++ and the EasyX graphics library. This project demonstrates basic game development concepts including graphics rendering, character movement, combat mechanics, and inventory management.

## ✨ Features

- **Smooth Character Movement**: Navigate the game world using multiple input methods
- **Combat System**: Engage in battles with enemies using attack commands
- **Inventory Management**: Collect and manage items throughout your adventure
- **Collision Visualization**: Debug mode to display collision boundaries (F1)
- **Intuitive Controls**: Support for both keyboard and mouse input

## 📋 Requirements

- **C++ Compiler**:
  - MinGW-w64 (for GCC)
  - Visual Studio 2019 or later
- **EasyX Graphics Library**: Latest version recommended
- **Windows OS**: EasyX is Windows-only

## 🚀 Installation & Setup

### 1. Clone the Repository
```bash
git clone https://github.com/jeoor/My_Rpg.git
cd My_Rpg
```

### 2. Install EasyX
- Download EasyX from the [official website](http://easyx.cn/)
- Run the installer and follow the setup instructions
- Alternatively, for Visual Studio, you can install via NuGet:
  ```
  Install-Package EasyX
  ```

### 3. Configure Your Development Environment

#### For Visual Studio:
1. Open the project folder in Visual Studio
2. Ensure EasyX headers and libraries are in your include/lib paths
3. Build the solution (Ctrl+Shift+B)

#### For Code::Blocks/MinGW:
1. Create a new project and add all source files
2. Configure linker settings to include EasyX libraries
3. Build and run

### 4. Build and Run
```bash
# If using Makefile (if provided)
make
./myrpg.exe
```

## 🎮 Controls

| Action | Keyboard | Mouse |
|--------|----------|-------|
| Move | WASD / Arrow Keys | Click to move |
| Attack | J | Right Click |
| Toggle Collision View | F1 | - |
| Inventory | I | - |
| Menu/Exit | ESC | - |

## 🏗️ Project Structure

```
My_Rpg/
├── 📁 MyRpg/                # Main project folder (contains source code and resources)
│   ├── ...                 # Source code files
│   └── 📄 resource.h       # Resource definition header file
├── 📄 MyRpg.sln            # Visual Studio Solution file
├── 📁 screenshot/          # Game screenshots
├── 📄 README.md            # Project documentation
├── 📄 LICENSE              # MIT License file
├── 📄 SECURITY.md          # Security policy document
├── 📄 .gitignore           # Git ignore rules
└── 📄 .gitattributes       # Git attribute configuration
```

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

Please ensure your code follows the existing style and includes appropriate documentation.

## 📝 To-Do & Future Features

- Dynamic Camera
- More Enemies and Weapons

## 🐛 Troubleshooting

**Common Issues:**

1. **"EasyX.h not found"**: Ensure EasyX is properly installed and included in your project's include path.

2. **Linker errors**: Verify that EasyX libraries are linked correctly in your project settings.

3. **Graphics not displaying**: Check if your system supports the graphics mode being used.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **EasyX Team** for providing an accessible graphics library for C++ developers
- The game development community for tutorials and inspiration
- Contributors and testers who help improve this project

---

**Enjoy your adventure in MyRPG!** For questions or feedback, please open an issue on the GitHub repository.

---

*Note: This is a learning project and may contain bugs or incomplete features. All contributions to improve the game are appreciated!*