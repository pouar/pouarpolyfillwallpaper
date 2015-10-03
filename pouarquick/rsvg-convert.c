#include <locale.h>
#include <librsvg/rsvg.h>
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

	GError *error = NULL;
	RsvgHandle *rsvg = NULL;
	surface[i] = NULL;
	cairo_t *cr = NULL;
	RsvgDimensionData dimensions;

	/* Set the locale so that UTF-8 filenames work */
	setlocale(LC_ALL, "");


	rsvg_set_default_dpi_x_y (-1, -1);
	
	

	rsvg = rsvg_handle_new_from_file(input,&error);


	rsvg_handle_get_dimensions (rsvg, &dimensions);
	width=(width<1)?(double)height/(double)dimensions.height*dimensions.width:width;
	height=(height<1)?(double)width/(double)dimensions.width*dimensions.height:height;
	double zoom=(width>height)?(double)width/(double)dimensions.width:(double)height/(double)dimensions.height;
	surface[i] = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);

	cr = cairo_create (surface[i]);
	cairo_scale(cr,zoom,zoom);
	rsvg_handle_render_cairo (rsvg, cr);
    g_object_unref (rsvg);
    cairo_destroy (cr);
	
	return 1;
}