﻿using CryEngine.Common;

using CryEngine.EntitySystem;

namespace CryEngine
{
	using RayFlags = rwi_flags;
	using EntityQueryFlags = entity_query_flags;

	/// <summary>
	/// Helper class for performing Ray casts.
	/// </summary>
	public static class Ray
	{
		private static RayFlags _rayFlags = unchecked((RayFlags)(((uint)((int)geom_flags.geom_colltype_ray << (int)rwi_flags.rwi_colltype_bit) | (int)rwi_flags.rwi_colltype_any | (10 & (int)rwi_flags.rwi_pierceability_mask) | ((int)geom_flags.geom_colltype14 << (int)rwi_flags.rwi_colltype_bit))));

		public static RaycastHit Cast(Vector3 position, Vector3 direction, float distance, EntityQueryFlags flags, RayFlags rayFlags, int maxHits)
		{
			var rayHit = new ray_hit();
			var hits = Engine.PhysicalWorld.RayWorldIntersection(position, direction * distance, (int)flags, (uint)rayFlags, rayHit, maxHits);
			return new RaycastHit(rayHit, hits);
		}

		public static RaycastHit Cast(Vector3 position, Vector3 direction)
		{
			return Cast(position, direction, float.PositiveInfinity, EntityQueryFlags.ent_all, _rayFlags, 1);
		}

		public static RaycastHit Cast(Vector3 position, Vector3 direction, float distance)
		{
			return Cast(position, direction, distance, EntityQueryFlags.ent_all, _rayFlags, 1);
		}
	}
}
