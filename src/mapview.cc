/*
 * Copyright (C) 2002-2004, 2006 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "interactive_base.h"
#include "interactive_player.h"
#include "map.h"
#include "mapview.h"
#include "mapviewpixelfunctions.h"
#include "overlay_manager.h"
#include "player.h"
#include "rendertarget.h"
#include "wlapplication.h"

/*
===============
Map_View::Map_View

Initialize
===============
*/
Map_View::Map_View(UIPanel *parent, int x, int y, uint w, uint h, Interactive_Base *player)
	: UIPanel(parent, x, y, w, h)
{
	m_intbase = player;
	m_viewpoint.x = m_viewpoint.y = 0;
	m_dragging = false;
   	m_complete_redraw_needed = true;
}


/*
===============
Map_View::~Map_View

Cleanups
===============
*/
Map_View::~Map_View(void)
{
}


/*
===============
Map_View::warp_mouse_to_field

Moves the mouse cursor so that it is directly above the given field
===============
*/
void Map_View::warp_mouse_to_field(Coords c)
{
	int x, y;

	MapviewPixelFunctions::get_save_pix(m_intbase->map(), c, x, y);
	x -= m_viewpoint.x;
	y -= m_viewpoint.y;

	if(x>=get_w() || y>=get_h()) {
      // The user has scrolled the field outside
      // the viewable area, he most surely doesn't
      // want to jump there
      return;
   }

   if(x<=0) { warp_mouse_to_field(Coords(c.x+m_intbase->get_map()->get_width(),c.y)); return; }
   if(y<=0) { warp_mouse_to_field(Coords(c.x, c.y+m_intbase->get_map()->get_height())); return; }

   set_mouse_pos(x, y);
}


/*
===============
Map_View::draw

This is the guts!! this function draws the whole
map the user can see. we spend a lot of time
in this function
===============
*/
void Map_View::draw(RenderTarget* dst)
{
   // Check if the view has changed in a game
	if
		(dynamic_cast<const Game * const>(m_intbase->get_egbase())
		 and
		 static_cast<Interactive_Player*>
		 (m_intbase)->get_player()->has_view_changed())
		m_complete_redraw_needed = true;

	m_intbase->get_map()->get_overlay_manager()->load_graphics();
   dst->rendermap
		(*m_intbase->get_egbase(),
		 m_intbase->get_visibility(),
		 m_viewpoint,
		 m_complete_redraw_needed);
   m_complete_redraw_needed = false;
}


/*
===============
Map_View::set_viewpoint

Set the viewpoint to the given pixel coordinates
===============
*/
void Map_View::set_viewpoint(Point vp)
{
	if (vp == m_viewpoint)
		return;

	MapviewPixelFunctions::normalize_pix(m_intbase->map(), vp);
	m_viewpoint = vp;

	warpview.call(m_viewpoint.x, m_viewpoint.y);

   m_complete_redraw_needed = true;
}

/** Map_View::handle_mouseclick(uint btn, bool down, int x, int y)
 *
 * Mouseclicks on the map:
 * Right-click: enable/disable dragging
 * Left-click: field action window
 */
bool Map_View::handle_mouseclick(uint btn, bool down, int x, int y)
{
	if (btn == MOUSE_LEFT)
	{
		if (down) {
         track_fsel(x, y);

			fieldclicked.call();
		}
	}
	else if (btn == MOUSE_RIGHT)
	{
		if (down) {
			m_dragging = true;
			grab_mouse(true);
			WLApplication::get()->set_mouse_lock(true);
		} else if (m_dragging) {
			WLApplication::get()->set_mouse_lock(false);
			grab_mouse(false);
			m_dragging = false;
		}
	}

	return true;
}


/*
===============
Map_View::handle_mousemove

Scroll the view according to mouse movement.
===============
*/
void Map_View::handle_mousemove(int x, int y, int xdiff, int ydiff, uint btns)
{
   if (!(btns & (1<<MOUSE_RIGHT)))
		m_dragging = false;

	if (m_dragging)
	{
		set_rel_viewpoint(Point(xdiff, ydiff));
	}

	if (!m_intbase->get_fieldsel_freeze())
		track_fsel(x, y);

	g_gr->update_fullscreen();
}


/*
===============
Map_View::track_fsel(int mx, int my)

Move the fsel to the given mouse position.
Does not honour fieldsel freeze.
===============
*/
void Map_View::track_fsel(int mx, int my)
{
	FCoords fsel;

	Coords c = MapviewPixelFunctions::calc_node_and_triangle
		(*m_intbase->get_map(), m_viewpoint.x + mx, m_viewpoint.y + my).node;
	fsel = FCoords(c, m_intbase->get_map()->get_field(c));
	// Apply the new fieldsel
	m_intbase->set_fieldsel_pos(fsel);
}
