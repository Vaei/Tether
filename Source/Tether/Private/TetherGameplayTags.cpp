// Copyright (c) Jared Taylor. All Rights Reserved


#include "TetherGameplayTags.h"

namespace FTetherGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Tether_Solver, "Tether.Solver");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Solver_Physics, "Tether.Solver.Physics");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Solver_Physics_Linear, "Tether.Solver.Physics.Linear");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Solver_Physics_Angular, "Tether.Solver.Physics.Angular");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Solver_Contact, "Tether.Solver.Contact");
	
	UE_DEFINE_GAMEPLAY_TAG(Tether_Shape, "Tether.Shape");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Shape_AxisAlignedBoundingBox, "Tether.Shape.AxisAlignedBoundingBox", "AABBs are simpler to compute, as they only require finding the minimum and maximum extents of the object along the coordinate axes. Because they are axis-aligned, checking for overlaps between two AABBs is computationally cheap");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Shape_OrientedBoundingBox, "Tether.Shape.OrientedBoundingBox", "Checking for overlaps between two OBBs is more computationally expensive than AABB, often requiring separating axis tests or other geometric methods.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Shape_BoundingSphere, "Tether.Shape.BoundingSphere", "The Bounding Sphere is extremely simple to compute. It is defined by a center point and a radius, which can be derived from the furthest point from the center of the object.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Shape_Capsule, "Tether.Shape.Capsule", "Capsules are more complex than spheres due to their elongated shape, but they are simpler than boxes (OBB) when it comes to collision detection. Collision detection for capsules typically involves checking both the cylindrical part and the hemispherical ends, which is more complex than sphere collision detection but still relatively efficient.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Shape_Cone, "Tether.Shape.Cone", "The cone is more complex than both the sphere and capsule because of its narrowing shape and the need to handle the vertex (tip) in collision detection. Detecting collisions with a cone involves understanding its varying radius along the height and considering its angular properties, making it computationally more demanding.");
}