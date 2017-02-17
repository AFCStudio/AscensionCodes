// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

using CryEngine.UI.Components;
using System.Runtime.Serialization;

namespace CryEngine.UI
{
	/// <summary>
	/// Represents a simple Panel element.
	/// </summary>
	[DataContract]
	public class Panel : UIElement
	{
		public Image Background { get; private set; } ///< Optional Background image for the Panel.

		/// <summary>
		/// Called by framework. Do not call directly.
		/// </summary>
		public override void OnAwake()
		{
			Background = AddComponent<Image>();
		}
	}
}
