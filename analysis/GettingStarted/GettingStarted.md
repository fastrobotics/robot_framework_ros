[Analysis](../Analysis.md)
- [Getting Started](#getting-started)
- [Setup](#setup)
- [Scripts](#scripts)
- [References](#references)

# Getting Started
# Setup
1. Add thefollowing to your environment variables (such as in ~/.bashrc):
```bash
export MESA_GL_VERSION_OVERRIDE=3.0; # Scilab Java Fix
```
2. Start scilab from command line:
```bash
scilab
```
3. Install the following toolboxes from inside scilab:
```scilab
atomsInstall("xcos_code_generator")
```
4. Restart scilab

# Scripts
| Script                                   | Purpose                             |
| ---------------------------------------- | ----------------------------------- |
| [HelloWorld.sce](scripts/HelloWorld.sce) | Simple plot of a sin wave           |
| [Filter.sce](scripts/Filter.sce)         | Design a filter and plot the result |
| [Animation.sce](scripts/Animation.sce)   | Animate a moving point              |

# References
- [Scilab Tutorials](https://www.scilab.org/tutorials)