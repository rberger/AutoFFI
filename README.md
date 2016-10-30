Truss
=====

> A library for deducing program structure information.

Truss is a library which allows you to query a program's structure within your
own software, without relying on external compilers or parsers. Truss does the
heavy lifting of selecting the right compiler for the job, and returns things
such as a list of symbols and type information of a set of sources to your
program.

The following is an example of a program that can be parsed by Truss:


```c

// Example taken from Chipmunk2D physics library
// https://github.com/slembcke/Chipmunk2D

// ...

struct cpBody {
	// Integration functions
	cpBodyVelocityFunc velocity_func;
	cpBodyPositionFunc position_func;
	
	// mass and it's inverse
	cpFloat m;
	cpFloat m_inv;
	
	// moment of inertia and it's inverse
	cpFloat i;
	cpFloat i_inv;
	
	// center of gravity
	cpVect cog;
	
	// position, velocity, force
	cpVect p;
	cpVect v;
	cpVect f;
	
	// Angle, angular velocity, torque (radians)
	cpFloat a;
	cpFloat w;
	cpFloat t;
	
	cpTransform transform;
	
	cpDataPointer userData;
	
	// "pseudo-velocities" used for eliminating overlap.
	// Erin Catto has some papers that talk about what these are.
	cpVect v_bias;
	cpFloat w_bias;
	
	cpSpace *space;
	
	cpShape *shapeList;
	cpArbiter *arbiterList;
	cpConstraint *constraintList;
	
	struct {
		cpBody *root;
		cpBody *next;
		cpFloat idleTime;
	} sleeping;
};

```

