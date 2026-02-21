# 🎮 FiveM Offset Dumper

Tool for **automatic extraction of updated FiveM offsets**.

Developed by **Pedro Guarconi** and **Gabriel Cancella**.

---

## 📖 Overview

FiveM Offset Dumper automates the process of retrieving offsets used by FiveM, eliminating the need for manual analysis after every update.

The tool scans the running process and generates a clean, organized list of offsets ready to use.

---

## ✨ Features

- Automatic offset dumping
- Compatible with recent FiveM updates
- Clean and organized output structure
- Fast and lightweight execution
- Easy integration with other projects

---

## 🧱 Technologies

- **C++**
- **Windows API**
- Memory analysis techniques

---

## 📦 Dependencies

This project uses:

- **KieroHook** — lightweight API hooking library  
- **ImGui** — graphical interface library  

All dependencies are already included.

---

## 🖥️ Requirements

- Windows 10 or newer
- **Visual Studio 2022**
- MSVC Toolset v143
- Build target: **Release x64**

---

## 🚀 Build Instructions

### 1. Clone the repository

```bash
git clone https://github.com/zljxl/Educational-Memory-Finder
```

### 2. Open in Visual Studio

Open the `.sln` file using **Visual Studio 2022**.

### 3. Compile

Select:

```
Release | x64
```

Then build the solution.

---

## ▶️ How to Use

### 1. Prepare FiveM

Navigate to your FiveM directory and rename:

```
adhesive.dll → adhesive.dll.bkp
```

---

### 2. Start FiveM

Launch FiveM and wait until it is fully loaded.

---

### 3. Inject the dumper

Use a DLL injector such as **Extreme Injector** (or any trusted injector):

- Run the injector as Administrator
- Select the compiled dumper DLL
- Inject into the **FiveM process**

⚠️ Make sure to select the correct FiveM process.

---

### 4. Inform the FiveM version

After injection, provide the FiveM build/version you are using when prompted.  
This ensures correct offset mapping.

---

### 5. Output

Offsets will be generated automatically at:

```
offsets.txt
```

---

## 📄 Example Output

```
World: 0x2476F50
ReplayInterface: 0x1F42028
Viewport: 0x1F9A3D0
LocalPlayer: 0x8
```

---

## ⚠️ Disclaimer

This project is provided **for educational and research purposes only**.

The authors are not responsible for any misuse.

---

## 👨‍💻 Authors

**Pedro Guarconi**  
**Gabriel Cancella**
