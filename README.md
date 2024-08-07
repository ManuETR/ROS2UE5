
<p align="center">
<img width="322" alt="ROS2UE5" src="https://github.com/user-attachments/assets/ce7b8efe-06dc-42d5-a6b1-5032156b9079" />
</p>

# ROS2UE5
![Under Construction](https://img.shields.io/badge/status-under%20construction-orange?logo=vlcmediaplayer&logoColor=ffffff)
![Author](https://img.shields.io/badge/author-Manuel%20Eiter-blue)
![Using Unreal Engine](https://img.shields.io/badge/using-Unreal%20Engine-purple?logo=unrealengine)
![Using ROS2 Humble](https://img.shields.io/badge/using-ROS2%20Humble-green?logo=ros)
![Static Badge](https://img.shields.io/badge/Patiiiiiii-UNSAFE-red?logo=pipx&logoColor=red&logoSize=Auto)

## Overview

**ROS2UE5** is an Unreal Engine 5 plugin that enables the integration of ROS2, allowing for advanced simulation and interaction within Unreal Engine's virtual environment.

## Features

- Seamless integration between ROS2 and Unreal Engine 5.3.
- Support for various ROS2 messages and services.
- Real-time simulation and visualization.
- Easy-to-use API for developing robotics applications in Unreal Engine.

## Installation

### Prerequisites

- Unreal Engine 5.3
- ROS2 Humble
- ROS webbridge

### Steps

1. Clone this repository into your Unreal Engine project's `Plugins` directory:
    ```sh
    git clone https://github.com/ManuETR/ROS2UE5.git Plugins/ROS2UE5
    ```

2. Open your Unreal Engine project. The plugin should be automatically detected and enabled.

3. If necessary, enable the plugin manually via `Edit -> Plugins` and searching for "ROS2UE5".

## Usage

1. **Configure ROS2**:
   - Ensure your ROS2 environment is set up and running.
   - Source your ROS2 workspace:
     ```sh
     source /opt/ros/humble/setup.bash
     ```

2. **Add ROS2 Components**:
   - In Unreal Engine, add ROS2 components to your actors via the component panel.

3. **Send and Receive Messages**:
   - Use the provided API to publish and subscribe to ROS2 topics, and call services as needed.

## Example
### ROS2 Control r6bot
![grafik](https://github.com/ManuETR/ROS2UE5/assets/37251724/049e1def-84be-4781-805a-e4a2e36d0593)



```
TODO
```

## Acknowledgments

- [ROS2](https://index.ros.org/doc/ros2/)
- [Unreal Engine](https://www.unrealengine.com/)
- [URoboSim](https://www.robcog.org/)
