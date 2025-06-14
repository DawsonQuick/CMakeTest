#include "../include/app.h"


/*
* The goal with this project is to work on a particle engine using opengl
* 
* Ideas to implement:
*       Particle Structure
*            Will hold Position, Velocity, Acceleration, Lifetime
*       Emitter Interface
*            Will have multiple emitter types all inheriting from the base Emitter interface
*       Solver Interface
*            Will have multiple particle "Solvers" all inheriting from the base Solver interface
*       Rendering
*            Will used instanced rendering, it was recommended (chatgpt) to do not use indices for this and only use vertices due to indices adding an indirection layer on the GPU
*/
int main(){

    App app;

    return 0;
}