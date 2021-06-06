typedef float4 point;
typedef float4 vector;
typedef float4 color;
typedef float4 sphere;


vector
Bounce( vector in, vector n )
{
	vector out = in - n*(vector)( 2.*dot(in.xyz, n.xyz) );
	out.w = 0.;
	return out;
}

vector
BounceSphere( point p, vector v, sphere s )
{
	vector n;
	n.xyz = fast_normalize( p.xyz - s.xyz );
	n.w = 0.;
	return Bounce( v, n );
}

bool
IsInsideSphere( point p, sphere s )
{
	float r = fast_length( p.xyz - s.xyz );
	return  ( r < s.w );
}

kernel
void
Particle( global point *dPobj, global vector *dVel, global color *dCobj )
{
	const float4 G       = (float4) ( 0., -9.8, 0., 0. );
	const float  DT      = 0.1;
	const sphere Sphere1 = (sphere)( 0., -800., 0.,  600. );
	const sphere Sphere2 = (sphere)( -2000., -800., 0.,  1000. );
	const sphere Sphere3 = (sphere)( 2000., -800., 0.,  1000. );
	const sphere Sphere4 = (sphere)( 0., -800., -2000.,  1000. );
	const sphere Sphere5 = (sphere)( 0., -800., 2000.,  1000. );
	const sphere Sphere6 = (sphere)( 0., -8500., 0.,  6000. );
	int gid = get_global_id( 0 );

	// extract the position and velocity for this particle:
	point  p = dPobj[gid];
	vector v = dVel[gid];

	// remember that you also need to extract this particle's color
	// and change it in some way that is obviously correct


	// advance the particle:

	point  pp = p + v*DT + G*(point)( .5*DT*DT ); // p'
	vector vp = v + G*DT;						  // v'
	pp.w = 1.;
	vp.w = 0.;

	// test against the first sphere here:

	if( IsInsideSphere( pp, Sphere1 ) )
	{
		vp = BounceSphere( p, v, Sphere1 );
		pp = p + vp*DT + G*(point)( .5*DT*DT );
		dCobj[gid] = (color)(1,1,0,0);
	}

	// test against the second sphere here:
	if( IsInsideSphere( pp, Sphere2 ) )
	{
		vp = BounceSphere( p, v, Sphere2 );
		pp = p + vp*DT + G*(point)( .5*DT*DT );
		dCobj[gid] = (color)(1,0,0.25,0);
	}

	// test against the third sphere here:
	if( IsInsideSphere( pp, Sphere3 ) )
	{
		vp = BounceSphere( p, v, Sphere3 );
		pp = p + vp*DT + G*(point)( .5*DT*DT );
		dCobj[gid] = (color)(0,0,1,0);
	}

	// test against the fourth sphere here:
	if( IsInsideSphere( pp, Sphere4 ) )
	{
		vp = BounceSphere( p, v, Sphere4 );
		pp = p + vp*DT + G*(point)( .5*DT*DT );
		dCobj[gid] = (color)(0,1,1,0);
	}

	// test against the fifth sphere here:
	if( IsInsideSphere( pp, Sphere5 ) )
	{
		vp = BounceSphere( p, v, Sphere5 );
		pp = p + vp*DT + G*(point)( .5*DT*DT );
		dCobj[gid] = (color)(1,1,1,0);
	}

	// test against the sixth sphere here:
	if( IsInsideSphere( pp, Sphere6 ) )
	{
		vp = BounceSphere( p, v, Sphere6 );
		pp = p + vp*DT + G*(point)( .5*DT*DT );
		dCobj[gid] = (color)(0.65, 0.45, 3,0);
	}

	dPobj[gid] = pp;
	dVel[gid]  = vp;
}
