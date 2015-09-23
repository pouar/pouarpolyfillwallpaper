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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <glib/gi18n.h>
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


#include <glib.h>

G_BEGIN_DECLS

struct ImageBuffer {
    unsigned char* data;
    int width;
    int height;
    int stride;
};

typedef enum {
    RSVG_SIZE_ZOOM,
    RSVG_SIZE_WH,
    RSVG_SIZE_WH_MAX,
    RSVG_SIZE_ZOOM_MAX
} RsvgSizeType;

struct RsvgSizeCallbackData {
    RsvgSizeType type;
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

void
_rsvg_size_callback (int *width, int *height, gpointer data)
{
    struct RsvgSizeCallbackData *real_data = (struct RsvgSizeCallbackData *) data;
    double zoomx, zoomy, zoom;

    int in_width, in_height;

    in_width = *width;
    in_height = *height;

    switch (real_data->type) {
    case RSVG_SIZE_ZOOM:
        if (*width < 0 || *height < 0)
            return;

        *width = floor (real_data->x_zoom * *width + 0.5);
        *height = floor (real_data->y_zoom * *height + 0.5);
        break;

    case RSVG_SIZE_ZOOM_MAX:
        if (*width < 0 || *height < 0)
            return;

        *width = floor (real_data->x_zoom * *width + 0.5);
        *height = floor (real_data->y_zoom * *height + 0.5);

        if (*width > real_data->width || *height > real_data->height) {
            zoomx = (double) real_data->width / *width;
            zoomy = (double) real_data->height / *height;
            zoom = MIN (zoomx, zoomy);

            *width = floor (zoom * *width + 0.5);
            *height = floor (zoom * *height + 0.5);
        }
        break;

    case RSVG_SIZE_WH_MAX:
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
        break;

    case RSVG_SIZE_WH:
        if (real_data->width != -1)
            *width = real_data->width;
        if (real_data->height != -1)
            *height = real_data->height;
        break;

    default:
        g_assert_not_reached ();
    }

    if (real_data->keep_aspect_ratio) {
        int out_min = MIN (*width, *height);

        if (out_min == *width) {
            *height = in_height * ((double) *width / (double) in_width);
        } else {
            *width = in_width * ((double) *height / (double) in_height);
        }
    }
}


static void
display_error (GError * err)
{
    if (err) {
        g_printerr ("%s\n", err->message);
        g_error_free (err);
    }
}

static void
rsvg_cairo_size_callback (int *width, int *height, gpointer data)
{
    RsvgDimensionData *dimensions = data;
    *width = dimensions->width;
    *height = dimensions->height;
}


struct ImageBuffer runrsvg (int width, int height, const char *input)
{
    struct ImageBuffer imagebuffer;
    double x_zoom = 1.0;
    double y_zoom = 1.0;
    double zoom = 1.0;
    double dpi_x = -1.0;
    double dpi_y = -1.0;
    int keep_aspect_ratio = TRUE;
    GError *error = NULL;

    RsvgHandle *rsvg = NULL;
    cairo_surface_t *surface = NULL;
    cairo_t *cr = NULL;
    RsvgHandleFlags flags = RSVG_HANDLE_FLAGS_NONE;
    RsvgDimensionData dimensions;


    /* Set the locale so that UTF-8 filenames work */
    setlocale(LC_ALL, "");





        




    if (zoom != 1.0)
        x_zoom = y_zoom = zoom;

    rsvg_set_default_dpi_x_y (dpi_x, dpi_y);

        flags |= RSVG_HANDLE_FLAG_UNLIMITED;

        flags |= RSVG_HANDLE_FLAG_KEEP_IMAGE_DATA;

        GFile *file;
        GInputStream *stream;
        GFileInfo *file_info;
        gboolean compressed = FALSE;

        file = g_file_new_for_path (input);
        stream = (GInputStream *) g_file_read (file, NULL, &error);

        if ((file_info = g_file_query_info (file,
                                            G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE,
                                            G_FILE_QUERY_INFO_NONE,
                                            NULL,
                                            NULL))) {
            const char *content_type;
            char *gz_content_type;

            content_type = g_file_info_get_content_type (file_info);
            gz_content_type = g_content_type_from_mime_type ("application/x-gzip");
            compressed = (content_type != NULL && g_content_type_is_a (content_type, gz_content_type));
            g_free (gz_content_type);
            g_object_unref (file_info);
        }

        if (compressed) {
            GZlibDecompressor *decompressor;
            GInputStream *converter_stream;

            decompressor = g_zlib_decompressor_new (G_ZLIB_COMPRESSOR_FORMAT_GZIP);
            converter_stream = g_converter_input_stream_new (stream, G_CONVERTER (decompressor));
            g_object_unref (stream);
            stream = converter_stream;
        }

        if (stream == NULL)
            goto done;

        rsvg = rsvg_handle_new_from_stream_sync (stream, file, flags, NULL, &error);

    done:

        if (error != NULL) {
            fprintf (stderr, _("Error reading SVG:"));
            display_error (error);
            fprintf (stderr, "\n");
            exit (1);
        }

        /* in the case of multi-page output, all subsequent SVGs are scaled to the first's size */
        rsvg_handle_set_size_callback (rsvg, rsvg_cairo_size_callback, &dimensions, NULL);

            struct RsvgSizeCallbackData size_data;

            rsvg_handle_get_dimensions (rsvg, &dimensions);
            /* if both are unspecified, assume user wants to zoom the image in at least 1 dimension */
            if (width == -1 && height == -1) {
                size_data.type = RSVG_SIZE_ZOOM;
                size_data.x_zoom = x_zoom;
                size_data.y_zoom = y_zoom;
                size_data.keep_aspect_ratio = keep_aspect_ratio;
            }
            /* if both are unspecified, assume user wants to resize image in at least 1 dimension */
            else if (x_zoom == 1.0 && y_zoom == 1.0) {
                /* if one parameter is unspecified, assume user wants to keep the aspect ratio */
                if (width == -1 || height == -1) {
                    size_data.type = RSVG_SIZE_WH_MAX;
                    size_data.width = width;
                    size_data.height = height;
                    size_data.keep_aspect_ratio = keep_aspect_ratio;
                } else {
                    size_data.type = RSVG_SIZE_WH;
                    size_data.width = width;
                    size_data.height = height;
                    size_data.keep_aspect_ratio = keep_aspect_ratio;
                }
            } else {
                /* assume the user wants to zoom the image, but cap the maximum size */
                size_data.type = RSVG_SIZE_ZOOM_MAX;
                size_data.x_zoom = x_zoom;
                size_data.y_zoom = y_zoom;
                size_data.width = width;
                size_data.height = height;
                size_data.keep_aspect_ratio = keep_aspect_ratio;
            }

            _rsvg_size_callback (&dimensions.width, &dimensions.height, &size_data);

                surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                                                      dimensions.width, dimensions.height);

            cr = cairo_create (surface);


        rsvg_handle_render_cairo (rsvg, cr);
    imagebuffer.data=cairo_image_surface_get_data(surface);
    imagebuffer.width=cairo_image_surface_get_width(surface);
    imagebuffer.height=cairo_image_surface_get_height(surface);
    imagebuffer.stride=cairo_image_surface_get_stride(surface);


    return imagebuffer;
}