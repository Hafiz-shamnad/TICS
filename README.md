# Tronics 🚀

Welcome to **Tronics** 🎉 — a Git-inspired version control system designed to manage code, IoT devices, and CAD designs (e.g., STL, STEP files) in one seamless workflow! 🌐 Tronics combines powerful version control features with specialized IoT state tracking and CAD file versioning, setting the stage for **TronicsHub**, a future platform for collaboration and innovation. 🛠️

---

## Features ✨

### Version Control 📚

* **Git-like Power 🔧**: Commit, branch, merge, rebase, diff, log, stash, and tag (lightweight and annotated).
* **Diffing 🔍**: Compare text files, IoT states, and CAD metadata between commits.
* **Remote Syncing 🌍**: Push/pull support (coming soon for TronicsHub integration).

### IoT Integration 📡

* **Device Management 🖥️**: Track device states and configurations via MQTT.
* **Firmware Versioning 💾**: Version firmware binaries with SHA-1 hashing.
* **Device Groups 👥**: Manage multiple IoT devices as a single entity.
* **Event Logging 📝**: Automatically commit IoT state changes.

### CAD Handling 🖌️

* **File Support 🗂️**: Version STL, STEP, and IGES files with metadata extraction (e.g., vertices, faces, volume).
* **Compression 📦**: Store CAD files efficiently using zlib.
* **Visual Diffing 👀**: Compare geometry and metadata changes *(in development)*.
* **Preview Generation 🖼️**: Create thumbnails for CAD files *(planned)*.

### CLI Interface 🖱️

* **Commands**: `tronics init`, `tronics commit`, `tronics branch`, `tronics diff`, `tronics log`, `tronics stash`, `tronics tag`, `tronics add-device`, `tronics add-cad`, `tronics push/pull`.

---

## Installation 🛠️

### Prerequisites

* Python 3.8+ 🐍
* Install dependencies:

```bash
pip install paho-mqtt trimesh pythonocc-core vtk
```

* An MQTT broker (e.g., Mosquitto) for IoT functionality 📡

### Setup

```bash
# Clone the repository
git clone https://github.com/your-username/tronics.git
cd tronics

# Install dependencies
pip install -r requirements.txt

# Initialize a Tronics repository
python -m tronics init
```

---

## Usage 🚀

### Initialize a Repository 🏗️

```bash
python -m tronics init
```

Creates a `.tronics` directory to store objects, references, and configuration.

### Add IoT Devices 📡

```bash
python -m tronics add-device sensor1 --broker localhost --type temperature
```

Registers an IoT device and subscribes to its MQTT topics.

### Add CAD Files 🖌️

```bash
python -m tronics add-cad design.stl
```

Adds and parses a CAD file, storing it with metadata (e.g., vertices, volume).

### Commit Changes ✅

```bash
python -m tronics commit -m "Initial commit with IoT and CAD"
```

Commits staged files, IoT states, and CAD metadata.

### Create a Branch 🌿

```bash
python -m tronics branch dev
```

Creates a new branch named `dev`.

### View Commit History 📜

```bash
python -m tronics log --limit 5
```

Displays the last 5 commits with timestamps and authors.

### Diff Commits 🔍

```bash
python -m tronics diff <commit1> <commit2>
```

Compares text files, IoT states, and CAD metadata between two commits.

### Stash Changes 📌

```bash
python -m tronics stash
```

Saves uncommitted changes for later.

### Tag a Commit 🏷️

```bash
python -m tronics tag v1.0 --annotated
```

Creates an annotated tag for the current commit.

---

## Example Workflow 🛠️

```python
from tronics import TronicsRepo

repo = TronicsRepo("./tronics_repo")
repo.iot_manager.add_device("sensor1", {"broker": "localhost", "type": "temperature"})
repo.iot_manager.add_device_group("group1", ["sensor1"])
repo.iot_manager.update_firmware("sensor1", "firmware.bin")
repo.cad_manager.add_cad("design.stl")
repo.commit("Initial commit")
repo.branch("dev")
repo.tag("v1.0", annotated=True)
print(repo.log(limit=5))
```

---

## Project Structure 📂

```
tronics/
├── tronics/
│   ├── __init__.py
│   ├── repo.py      # Core version control 📚
│   ├── iot.py       # IoT device management 📡
│   ├── cad.py       # CAD file handling 🖌️
│   ├── api.py       # REST API (planned) 🌐
│   ├── cli.py       # CLI interface 🖱️
├── tests/
│   ├── test_repo.py
│   ├── test_iot.py
│   ├── test_cad.py
├── examples/
│   ├── sample.stl
│   ├── firmware.bin
├── requirements.txt
└── README.md
```

---

## Roadmap 🗺️

### Core Enhancements (Q3 2025) 🚧

* Implement merge conflict resolution 🔄
* Add advanced CAD diffing (geometry, layers) 🖼️
* Support CoAP for IoT alongside MQTT 📡

### TronicsHub Platform (Q4 2025) 🌐

* Develop REST API for repository hosting 🖥️
* Build web interface with React and Three.js for CAD visualization 🎨
* Add collaboration features (pull requests, comments) 👥

### Advanced Features (Q1 2026) 🚀

* IoT real-time monitoring dashboard 📊
* CAD marketplace for sharing designs 🛒
* Integration with 3D printing services 🖨️

---

## Contributing 🤝

We welcome contributions! Please follow these steps:

1. Fork the repository 🍴
2. Create a feature branch: `git checkout -b feature-name` 🌿
3. Commit your changes: `git commit -m "Add feature"` ✅
4. Push to the branch: `git push origin feature-name` 🚀
5. Open a pull request 📬

---

## License 📜

Apache License. See `LICENSE` for details.

---

## Contact 📧

For questions or feedback, open an issue on GitHub or reach out to [haafizshamnad@gmail.com](mailto:haafizshamnad@gmail.com).

> Tronics is the foundation for TronicsHub, a future platform for collaborative IoT and CAD development. Join us in building the future of engineering! 🚀
