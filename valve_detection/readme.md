# Valve Detection System for Rescue Robots

## Overview
This project is part of a rescue robot initiative designed to detect and close valves during search-and-rescue operations. It leverages the YOLOv8 object detection model to accurately identify valves in real-time using the robot's onboard camera. The system processes visual data to identify valve positions, enabling the robot to perform necessary actions autonomously. The detection results are visualized in **RViz** for easy monitoring and interaction.

Additionally, the system may be extended for detecting radioactive material in water pipes for the Enrich Challenge, though this part is not directly the responsibility of the project.

---

## Start
1. Navigate to the project directory:
   ```bash
   cd taurob_tracker/valve_detection
   ```

2. Start the container:
   ```bash
   bash run_valve_detection_container.sh
   ```

3. The system should now be running with YOLOv8 for valve detection and visualized in RViz.

---

## Features
- Valve detection using YOLOv8 object detection model.
- Real-time processing with onboard camera.
- Detection results visualized in RViz.
- Ability to autonomously identify valves and take appropriate actions.
- Potential for extension to radioactive material detection (Enrich Challenge).

---

## License
Licensed under the [MIT License](LICENSE).