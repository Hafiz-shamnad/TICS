# Tronics 🚀

Welcome to **Tronics** 🎉 — a Git-inspired version control system designed to manage code, IoT device data, and CAD designs in one seamless workflow! 🌐 Tronics combines robust version control with specialized IoT state tracking and CAD file versioning, setting the stage for **TronicsHub**, a future platform for collaboration and innovation. 🛠️

---

## Features ✨

### Version Control 📚

- **Git-like Power 🔧**: Commit, branch, merge, diff, log, stash, tag, and file history tracking.
- **Diffing 🔍**: Compare text files and staged changes between commits.
- **Status Checking 📊**: View staged and committed files with current branch details.
- **File Restoration 🔄**: Restore files from previous commits.

### IoT Integration 📡

- **Device Simulation 🖥️**: Track simulated IoT device states (e.g., temperature, pressure).
- **Event Logging 📝**: Automatically append IoT data to versioned files.
- **Device Management 🛠️**: Store IoT sensor data in the repository (planned expansion for MQTT).

### CAD Handling 🖌️

- **File Support 🗂️**: Version CAD event logs (e.g., draw_line events).
- **Event Logging 📝**: Track CAD operations in versioned files.
- **Metadata Tracking 📊**: Store CAD-related changes in the repository (planned support for STL/STEP files).

### CLI Interface 🖱️

**Available Commands:**

```bash
tronics init
tronics add
tronics commit
tronics log
tronics status
tronics diff
tronics restore
tronics branch
tronics checkout
tronics merge
tronics history
tronics stash
tronics pop
tronics list-stash
tronics tag
tronics list-tags
tronics iot
tronics cad
```

---

## Installation 🛠️

### Prerequisites

- A C compiler (e.g., `gcc`)
- Standard C libraries: `stdio.h`, `stdlib.h`, `string.h`, `sys/stat.h`, `unistd.h`, `time.h`, `dirent.h`
- POSIX-compliant system (Linux/macOS)
- No external dependencies required

### Setup

```bash
# Clone the repository
git clone https://github.com/your-username/tronics.git
cd tronics

# Compile the Tronics executable
gcc tronics.c -o tronics

# Move executable to a system path (optional)
sudo mv tronics /usr/local/bin/

# Initialize a Tronics repository
./tronics init myrepo
```

---

## Usage 🚀

### Initialize a Repository 🏗️

```bash
./tronics init myrepo
```

Creates a `.tronics` directory in `myrepo` to store objects, references, stash, stage, history, tags, and configuration.

### Add Files 📄

```bash
./tronics add sensor_data.txt
```

Stages a file for commit.

### Commit Changes ✅

```bash
./tronics commit -m "Initial commit with sensor data"
```

Commits staged files and logs the message.

### Simulate IoT Data 📡

```bash
./tronics iot
```

Appends simulated IoT sensor data to `sensor_data.txt`.

### Simulate CAD Events 🖌️

```bash
./tronics cad
```

Appends simulated CAD events to `cad_log.txt`.

### Create a Branch 🌿

```bash
./tronics branch dev
```

Creates a new branch.

### View Commit History 📜

```bash
./tronics log
```

Displays all commits with timestamps and messages.

### Diff Files 🔍

```bash
./tronics diff sensor_data.txt
```

Compares the working file with its staged version.

### Stash Changes 📌

```bash
./tronics stash
```

Saves staged changes.

### Tag a Commit 🏷️

```bash
./tronics tag v1.0
```

Creates a lightweight tag.

### View Status 📊

```bash
./tronics status
```

Shows staged/committed files and current branch.

---

## Example Workflow 🛠️

```bash
# Initialize repository
./tronics init testrepo
cd testrepo

# Add and commit a file
echo "sensor: active" > sensor_data.txt
../tronics add sensor_data.txt
../tronics commit -m "Initial sensor data"

# Simulate IoT and CAD data
../tronics iot
../tronics cad

# Create and switch to a branch
../tronics branch dev
../tronics checkout dev

# Stash and pop changes
echo "sensor: updated" >> sensor_data.txt
../tronics add sensor_data.txt
../tronics stash
../tronics pop

# Commit and view history
../tronics commit -m "Updated sensor data"
../tronics history sensor_data.txt

# Merge and tag
../tronics checkout main
../tronics merge dev
../tronics tag v1.0
```

---

## Project Structure 📂

```
tronics/
├── tronics.c          # Core version control implementation
├── test_tronics.sh    # Test script
├── README.md          # Documentation
├── examples/
│   ├── sensor_data.txt  # Sample IoT file
│   └── cad_log.txt      # Sample CAD event log
```

---

## Roadmap 🗺️

### Core Enhancements (Q3 2025)

- Merge conflict resolution
- STL/STEP file versioning
- MQTT IoT integration

### TronicsHub Platform (Q4 2025)

- Remote repository hosting
- Web interface
- Pull requests and collaboration tools

### Advanced Features (Q1 2026)

- Real-time IoT monitoring dashboard
- CAD diff/preview generation
- External IoT/CAD tool integration

---

## Contributing 🤝

We welcome contributions!

1. Fork the repo 🍴  
2. Create a feature branch `git checkout -b feature-name`  
3. Commit your changes `git commit -m "Add feature"`  
4. Push the branch `git push origin feature-name`  
5. Open a pull request 📬

---

## License 📜

Apache License 2.0. See [`LICENSE`](LICENSE) for details.

---

## Contact 📧

Open an issue or email [haafizshamnad@gmail.com](mailto:haafizshamnad@gmail.com)

> Tronics is the foundation for **TronicsHub**, a future platform for collaborative IoT and CAD development. Join us in building the future of engineering! 🚀