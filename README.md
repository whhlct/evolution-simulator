# Evolution Simulator

## Overview
`Evolution Simulator` is a unique C++ project that creates a dynamic simulation of predator and prey interactions through artificial intelligence training methods. Employing SFML, the application visualizes these interactions via simple circles that represent the predator and prey entities. 

The AI controlling these entities is trained through the use of the NeuroEvolution of Augmenting Topologies (NEAT) algorithm or Deep Q-Network (DQN) reinforcement learning. A significant aspect of this simulation is the sensory input for the AI, which is represented by ray-cast vision.

## Dependencies
- C++ Compiler (GCC recommended)
- Recent release of SFML

## Installation
1. Clone the repository to your local machine.
2. Install SFML on your system. If you're using GCC, you can install SFML via the package manager.
3. Compile the project using your C++ compiler.

## Running the Simulator
After compiling the simulator, run the simulator executable. The simulator will begin running the predator-prey simulation, with the AI learning and adapting over time.

## AI Training
The AI agents for both predators and prey are trained through one of two methods: The NEAT algorithm or DQN reinforcement learning. 

### NEAT Training
NEAT training leverages a method of evolving artificial neural networks, primarily focusing on adjusting the topology architecture to enhance the AI's performance over time. 

### DQN Reinforcement Learning
The DQN reinforcement learning employs a policy to decide the next action based on the current state, refining its decision-making capabilities as it continuously learns from the environment.

Both of these methods utilize ray-cast vision for sensory input, which provides the AI with data about its surroundings and allows it to make more informed decisions.

## Contributing
Community contributions can help improve the Evolution Simulator. If you wish to contribute, kindly fork the repository and propose a pull request.

## License
This project is licensed under the GPL-3.0 License. Please see the `LICENSE` file for more details.

We hope you enjoy using the Evolution Simulator and we look forward to seeing the innovative ways in which you implement and expand upon it!

## Troubleshooting & Support
If you encounter any problems or have any queries about this project, be sure to file an issue in this repository. We appreciate your feedback and will respond to your issue promptly.
