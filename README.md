# TOC-suite: Theory of Computation Visualizer & Solver

A lightweight C++ and SFML tool to convert NFAs to DFAs, minimize DFAs, and interactively visualize the state transition graphs.

---

## 🛠️ Compilation & Execution

### Prerequisites
* `g++` compiler
* SFML libraries (`libsfml-dev`)

### Compile
```bash
g++ script.cpp helper_modules/visualizer.cpp -lsfml-graphics -lsfml-window -lsfml-system
```

### Run
```bash
./a.out
```

---

## 📁 Input Format

All input files are located inside the `inputs/` folder:

* **`inputs/alphabet.txt`**: Space-separated symbols representing the input alphabet.
  ```text
  a b c
  ```
* **`inputs/start_state.txt`**: The starting state.
  ```text
  q0
  ```
* **`inputs/final_states.txt`**: Space-separated list of accepting/final states.
  ```text
  q3 q4 q7
  ```
* **`inputs/input.txt`**: State transitions in the format `from_state symbol to_state` (one per line).
  ```text
  q0 a q1
  q0 b q2
  q1 a q3
  ```

---

## 🎮 Interactive GUI & Keybindings

When the visualizer window opens:

* **Left Click + Drag**: Click and drag any state node to rearrange the graph layout.
* **`D`**: Toggle between **Dark Mode** and **Light Mode**.
* **`E`**: Export and save the current graph layout as a `.jpg` image to the `outputs/` folder.
* **Auto-Save**: The initial graph is also saved automatically to `outputs/` upon rendering.

---

## 🚀 Future Roadmap

This project is planned to expand into a complete **educational suite for Theory of Computation (TOC) teachers and students**, including:
* Step-by-step interactive algorithm visualizers (Equivalence table filling, NFA $\rightarrow$ DFA subset construction step-by-step).
* Regular Expression (Regex) to $\epsilon$-NFA conversions.
* Pushdown Automata (PDA) and Turing Machine simulation modules.
* Classroom-ready exportable visual aids, slides, and test problem generators.
