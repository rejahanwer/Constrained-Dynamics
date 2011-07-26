Constrained Dynamics simulation using Legrange Mechanics
By Rejah Anuvar
================
- Constrained Dynamics Simulation of a string with beads.
- Demonstrates Pin, Rigid, Ring constraints
- Baumgarte Stabilization implemented
- Damping Forces present
- Euler/RK4 Integration options
- Also has self-collission detection -
   - Detects collission between any two beads and adds penalty forces.
   - The impact forces are not well dealt , but the effect can be seen.


Build/Exec:
$ make
$ ./string

Additional Libraries:
GSL 	  (-lgsl)
GSL CBLAS (-lgslcblas)

Keyboard Control - 
Arrow keys - Add forces to the beads.
P - Pause
T - Toggles Euler/RK4
D - Toggle Damping On/Off


Results:
 - The links extended slightly without baumgarte stabilization.
 - Without damping, the simulation goes unstable.
