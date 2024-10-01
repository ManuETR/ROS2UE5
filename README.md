
<p align="center">
<img width="322" alt="ROS2UE5" src="https://github.com/user-attachments/assets/ce7b8efe-06dc-42d5-a6b1-5032156b9079" />
</p>

![Under Construction](https://img.shields.io/badge/status-under%20construction-orange?logo=vlcmediaplayer&logoColor=ffffff)
![Author](https://img.shields.io/badge/author-Manuel%20Eiter-blue)
![Using Unreal Engine](https://img.shields.io/badge/using-Unreal%20Engine-purple?logo=unrealengine)
![Using ROS2 Humble](https://img.shields.io/badge/using-ROS2%20Humble-green?logo=ros)
![Static Badge](https://img.shields.io/badge/Patiiiiiii-UNSAFE-red?logo=pipx&logoColor=red&logoSize=Auto)

---

# ROS2UE5

**ROS2UE5** is a plugin that enables the simulation of Cyber-Physical Systems (CPS) controlled by ROS2 (Robot Operating System 2) within Unreal Engine 5.3. By integrating ROS2 with Unreal Engine, this plugin provides a powerful environment to simulate and visualize complex robotic systems in real time.

The plugin is built around four key modules:

- **ConfigLoader**: Handles the loading and configuration of ROS2 communication settings.
- **UROSBridge**: Facilitates the communication between Unreal Engine and ROS2 via the ROS web bridge.
- **URoboSim**: Provides simulation components specifically for robotic systems.
- **URoboSimEditor**: Tools and features for editing and managing robotic simulations directly in the Unreal Editor.

## Table of Contents
- [Requirements](#requirements)
- [Installation and Setup Guide](#installation-and-setup-guide)
  - [1. Install ROS2 Humble Hawksbill](#1-install-ros2-humble-hawksbill)
  - [2. Install Unreal Engine 5.3](#2-install-unreal-engine-53)
  - [3. Install and Run ROS Bridge Server](#3-install-and-run-ros-bridge-server)
  - [4. Add ROS2UE5 Plugin to Your Unreal Project](#4-add-ros2ue5-plugin-to-your-unreal-project)
  - [5. Optional: Install Additional Tools](#5-optional-install-additional-tools)
    - [5.1 ROS2UE5 Automation Tools](#51-ros2ue5-automation-tools)
    - [5.2 SeqLog Integration](#52-seqlog-integration)
  - [6. Testing the Plugin](#6-testing-the-plugin)
- [Features](#features)
- [Contributing](#contributing)
- [License](#license)

## Requirements

Before using the plugin, ensure you have the following installed:

1. **ROS2 (Humble Hawksbill)**: ROS2 is required for handling robotic systems and middleware. You can install ROS2 Humble from the [official ROS installation page](https://docs.ros.org/en/humble/Installation.html).
   
2. **Unreal Engine 5.3**: You will need a UE5 project with version 5.3. [Download Unreal Engine here](https://www.unrealengine.com/en-US/download).

3. **ROS Web Bridge**: The plugin uses `rosbridge_suite` for web communication between ROS and Unreal Engine. You can find the `rosbridge_suite` here: [ROS Bridge Suite GitHub](https://github.com/RobotWebTools/rosbridge_suite?tab=readme-ov-file).

## Installation and Setup Guide

### 1. Install ROS2 Humble Hawksbill

Follow the official guide for installing ROS2 Humble:
- [ROS2 Humble Installation Instructions](https://docs.ros.org/en/humble/Installation.html)

Once installed, ensure that your environment is correctly set up to use ROS2. Test the installation by running:
```bash
source /opt/ros/humble/setup.bash
ros2 --version
```

### 2. Install Unreal Engine 5.3

You can download and install Unreal Engine from the [Epic Games Launcher](https://www.unrealengine.com/en-US/download). Create a project in Unreal Engine 5.3, or open an existing one to integrate the **ROS2UE5** plugin.

### 3. Install and Run ROS Bridge Server

Install the `rosbridge_suite` package using the following:
```bash
sudo apt install ros-humble-rosbridge-suite
```

Once installed, run the ROS bridge server with:
```bash
ros2 launch rosbridge_server rosbridge_websocket_launch.xml
```

### 4. Add ROS2UE5 Plugin to Your Unreal Project

1. Download or clone the **ROS2UE5** repository to your local machine:
   ```bash
   git clone https://github.com/ManuETR/ROS2UE5.git
   ```

2. Copy the plugin folder into your Unreal project’s `Plugins` directory:
   ```
   <YourUnrealProject>/Plugins/ROS2UE5
   ```

3. Open your project in Unreal Engine and activate the **ROS2UE5** plugin:
   - Go to **Edit** -> **Plugins**.
   - Search for "ROS2UE5" and enable the plugin.
   - Restart Unreal Engine if prompted.

### 5. Optional: Install Additional Tools

These repositories are not required but can greatly extend and automate your simulation workflows:

#### 5.1 ROS2UE5 Automation Tools

The **ROS2UE5 Automation Tools** repository provides utilities to streamline the setup, creation, and launching of Unreal Engine simulations using configuration files for easy automation. 

Clone the repository:
```bash
git clone https://github.com/ManuETR/ROS2UE5-tools.git
```
This will allow for rapid simulation setup with predefined configurations and automation scripts. Refer to the `README` in this repository for detailed instructions on usage.

#### 5.2 SeqLog Integration

The **SeqLog Integration** repository extends **ROS2UE5** by providing an interface to the SeqLog data sink, allowing retrieval of detailed simulation data and logging.

Clone the repository:
```bash
git clone https://github.com/ManuETR/SeqLog.git
```
Follow the setup instructions in the repository to integrate SeqLog with **ROS2UE5**, enabling advanced data collection from your simulations.

### 6. Testing the Plugin

After setup, you can test the ROS2 and Unreal Engine integration by following these steps:

1. Launch the ROS bridge server:
   ```bash
   ros2 launch rosbridge_server rosbridge_websocket_launch.xml
   ```

2. Open your Unreal project and run a scene that includes ROS2 simulation components (such as URoboSim).

3. Verify the ROS2 topics, services, and messages are correctly being transmitted by monitoring ROS topics with:
   ```bash
   ros2 topic list
   ```

## Features

- Seamless integration of ROS2 and Unreal Engine simulations.
- Support for ROS2 communication via WebSockets (using ROS bridge).
- Configurable robotic simulation environment using **URoboSim**.
- Editor support via **URoboSimEditor** for easy management of robot configurations and interactions.
- Optional automation tools for easy simulation setup and control.

## Acknowledgments

- [ROS2](https://index.ros.org/doc/ros2/)
- [Unreal Engine](https://www.unrealengine.com/)
- [URoboSim](https://www.robcog.org/)


---
