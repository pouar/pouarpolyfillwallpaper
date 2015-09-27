/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set sw=4 sts=4 ts=4 expandtab: */
/*

rsvg-convert.c: Command line utility for exercising rsvg with cairo.

Copyright (C) 2005 Red Hat, Inc.
Copyright (C) 2005 Dom Lachowicz <cinamod@hotmail.com>
Copyright (C) 2005 Caleb Moore <c.moore@student.unsw.edu.au>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this program; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

Authors: Carl Worth <cworth@cworth.org>, 
			Caleb Moore <c.moore@student.unsw.edu.au>,
			Dom Lachowicz <cinamod@hotmail.com>
*/


#include <stdlib.h>
#include <locale.h>
#include <librsvg/rsvg.h>


/*
Copyright (C) 2000 Eazel, Inc.
Copyright (C) 2002 Dom Lachowicz <cinamod@hotmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this program; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

Author: Raph Levien <raph@artofcode.com>
*/



G_BEGIN_DECLS


struct RsvgSizeCallbackData {
	double x_zoom;
	double y_zoom;
	gint width;
	gint height;
	gboolean keep_aspect_ratio;
};
G_GNUC_INTERNAL
void _rsvg_size_callback (int *width, int *height, gpointer data);

G_END_DECLS
/*
Copyright (C) 2000 Eazel, Inc.
Copyright (C) 2002 Dom Lachowicz <cinamod@hotmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this program; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

Author: Raph Levien <raph@artofcode.com>
*/



#include <math.h>

void _rsvg_size_callback (int *width, int *height, gpointer data)
{
	struct RsvgSizeCallbackData *real_data = (struct RsvgSizeCallbackData *) data;
	double zoomx, zoomy, zoom;

	int in_width, in_height;

	in_width = *width;
	in_height = *height;

	if (*width < 0 || *height < 0)
		return;

	zoomx = (double) real_data->width / *width;
	zoomy = (double) real_data->height / *height;
	if (zoomx < 0)
		zoom = zoomy;
	else if (zoomy < 0)
		zoom = zoomx;
	else
		zoom = MIN (zoomx, zoomy);

	*width = floor (zoom * *width + 0.5);
	*height = floor (zoom * *height + 0.5);

	if (real_data->keep_aspect_ratio) {
		int out_min = MIN (*width, *height);

		if (out_min == *width) {
			*height = in_height * ((double) *width / (double) in_width);
		} else {
			*width = in_width * ((double) *height / (double) in_height);
		}
	}
}

static void display_error (GError * err)
{
	if (err) {
		g_printerr ("%s\n", err->message);
		g_error_free (err);
	}
}

static void rsvg_cairo_size_callback (int *width, int *height, gpointer data)
{
	RsvgDimensionData *dimensions = data;
	*width = dimensions->width;
	*height = dimensions->height;
}
cairo_surface_t *surface[2];

void cleanup(int i)
{
		
	cairo_surface_destroy (surface[i]);
}
unsigned char* svgbuf(int i)
{
	return cairo_image_surface_get_data(surface[i]);
}
int svgwidth(int i)
{
	return cairo_image_surface_get_width(surface[i]);
}
int svgheight(int i)
{
	return cairo_image_surface_get_height(surface[i]);
}
int svgstride(int i)
{
	return cairo_image_surface_get_stride(surface[i]);
}

int runrsvg (int width, int height, const char *input, int i)
{

	int keep_aspect_ratio = TRUE;
	GError *error = NULL;
	RsvgHandle *rsvg = NULL;
	surface[i] = NULL;
	cairo_t *cr = NULL;
	RsvgHandleFlags flags = RSVG_HANDLE_FLAGS_NONE;

	RsvgDimensionData dimensions;

	/* Set the locale so that UTF-8 filenames work */
	setlocale(LC_ALL, "");


	rsvg_set_default_dpi_x_y (-1, -1);
	flags |= RSVG_HANDLE_FLAG_UNLIMITED;
	flags |= RSVG_HANDLE_FLAG_KEEP_IMAGE_DATA;
	
	

	rsvg = rsvg_handle_new_from_file(input,&error);


	if (error != NULL) {
		fprintf (stderr, "Error reading SVG:");
		display_error (error);
		fprintf (stderr, "\n");
		exit (1);
	}

	/* in the case of multi-page output, all subsequent SVGs are scaled to the first's size */
	rsvg_handle_set_size_callback (rsvg, rsvg_cairo_size_callback, &dimensions, NULL);

	struct RsvgSizeCallbackData size_data;

	rsvg_handle_get_dimensions (rsvg, &dimensions);
	size_data.width = width;
	size_data.height = height;
	size_data.keep_aspect_ratio = keep_aspect_ratio;

	_rsvg_size_callback (&dimensions.width, &dimensions.height, &size_data);

	surface[i] = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, dimensions.width, dimensions.height);

	cr = cairo_create (surface[i]);

	rsvg_handle_render_cairo (rsvg, cr);
    g_object_unref (rsvg);
    cairo_destroy (cr);
	
	return 1;
}