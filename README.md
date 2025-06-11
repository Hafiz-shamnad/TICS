# TICS 🚀

Welcome to **TICS** 🎉 — a Git-inspired version control system designed to **Track**, **Integrate**, **Control**, and **Synchronize** code, IoT device data, and CAD designs in a seamless workflow. 🌐

---

## 🔢 TICS Explained

| **Letter** | **Meaning**                                                   |
| ---------- | ------------------------------------------------------------- |
| **T**      | Track: Monitors changes and progress in your projects.        |
| **I**      | Integrate: Combines different code versions or data smoothly. |
| **C**      | Control: Manages versions, branches, and updates efficiently. |
| **S**      | Synchronize: Aligns your work across devices or teams.        |

---

## ✨ Features

### Version Control 📚

* **Git-like Functionality**: Commit, branch, merge, diff, log, stash, tag, and history tracking.
* **Diffing**: Compare text files and staged changes across commits.
* **Status Checking**: View staged/committed files and current branch details.
* **File Restoration**: Revert files to previous commit states.

### IoT Integration 🛁

* **Device Simulation**: Tracks simulated IoT device states.
* **Event Logging**: Appends IoT data to versioned files.
* **Device Management**: Stores sensor data in the repository (MQTT support coming soon).

### CAD Handling 🎨

* **File Support**: Versions CAD logs (e.g., draw\_line events).
* **Event Logging**: Records CAD operations.
* **Metadata Tracking**: Captures CAD metadata (STL/STEP support planned).

### CLI Interface 🖱️

**Available commands:**

```
tics init         tics add        tics commit     tics log
tics status       tics diff       tics restore    tics branch
tics checkout     tics merge      tics history    tics stash
tics pop          tics list-stash tics tag        tics list-tags
tics iot          tics cad
```

---

## 🛠️ Installation

### Prerequisites

* GCC or any C compiler
* Standard C libraries: `stdio.h`, `stdlib.h`, etc.
* POSIX system (Linux/macOS)
* `make` utility

### Setup

```bash
git clone https://github.com/your-username/tics.git
cd tics
make
sudo mv tics /usr/local/bin/   # Optional
./tics init myrepo
```

---

## 🚀 Usage

### Initialize a Repository

```bash
./tics init myrepo
```

### Add Files

```bash
./tics add sensor_data.txt
```

### Commit Changes

```bash
./tics commit -m "Initial commit with sensor data"
```

### Simulate IoT and CAD

```bash
./tics iot
./tics cad
```

### Manage Branches

```bash
./tics branch dev
./tics checkout dev
```

### Stash and Pop

```bash
echo "sensor: updated" >> sensor_data.txt
./tics add sensor_data.txt
./tics stash
./tics pop
```

### History and Tags

```bash
./tics commit -m "Updated sensor data"
./tics history sensor_data.txt
./tics checkout main
./tics merge dev
./tics tag v1.0
```

---

## 📂 Project Structure

```
tics/
├── include/           # Header files
│   ├── branch_ops.h
│   ├── cad_ops.h
│   ├── commit_ops.h
│   ├── file_ops.h
│   ├── repo_ops.h
│   ├── stash_ops.h
│   ├── tag_ops.h
│   └── tics.h
├── src/               # C source files
│   ├── branch_ops.c
│   ├── cad_ops.c
│   ├── commit_ops.c
│   ├── file_ops.c
│   ├── main.c
│   ├── repo_ops.c
│   ├── stash_ops.c
│   ├── tag_ops.c
├── Makefile           # Build script
├── README.md          # This file
├── LICENSE            # Apache 2.0
├── test-tics.sh       # Test script
├── test.txt           # Sample file
```

---

## 🌍 Roadmap

### Q3 2025

* ✅ Automated merge conflict resolution
* ✅ STL/STEP file versioning
* ✅ MQTT integration for IoT

### Q4 2025

* ✅ **TICSHub**: remote hosting, web interface, pull requests

### Q1 2026

* ✅ Real-time IoT dashboard
* ✅ CAD file diffing & previews
* ✅ Integration with IoT/CAD tools

---

## 🧪 Testing

```bash
chmod +x test-tics.sh
./test-tics.sh
```

*Ensure the executable is named `tics`. Rename the test file if needed.*

---

## ⚠️ Troubleshooting

* **Command Not Found**: Use `./tics` or add to PATH.
* **Build Issues**: Run `make clean && make`.
* **Crashes**: Use `gdb ./tics` to debug.
* **Merge Conflicts**: Reset file metadata and retry.

---

## 🤝 Contributing

1. Fork the repo
2. Create a branch: `git checkout -b feature`
3. Commit: `git commit -m "Add feature"`
4. Push: `git push origin feature`
5. Open a Pull Request

---

## 📧 Contact

Email: **[haafizshamnad@gmail.com](mailto:haafizshamnad@gmail.com)**

> **TICS** is the foundation for **TICSLab**, a future-ready platform for collaborative IoT & CAD innovation. Join us in building the future of engineering! 🚀
