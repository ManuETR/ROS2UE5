This is a template for a `config.json` file that can be used to automatically launch a map and a robot inside the UnrealEditor

```json
{
    "environment": {
        "map": "PandaMap",
        "Weather": "sunny",
        "TimeOfDay": "noon"
    },
    "robots": [
        {
            "robot": "panda",
            "position": [0, 0, 5],
            "subscribers": [
                {
                    "type": "JointStateSubscriber",
                    "topic": "/joint/states"
                }
            ],
            "controllers": [
                {
                    "type": "JointController",
                    "mode": "kinematic"
                }
            ]
        }
    ],
    "Simulation": {
        "TimeScale": 1.0,
        "Duration": 300,
        "Physics": {
            "Gravity": -9.81,
            "Friction": 0.5
        }
    },
    "ros": {
        "bridge": {
            "IP": "127.0.0.1",
            "Port": 9090
        }
    },
    "logging": {
        "enable": true,
        "LogFrequency": 1,
        "LogDestination": {
            "Type": "Seq",
            "Endpoint": "http://localhost:5341"
        }
    }
}
```
