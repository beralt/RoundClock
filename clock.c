#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <gtk/gtk.h>
#include <cairo.h>

#include "clock.h"
#include "clock_date.h"

/* global declarations */

int main(int argc, char *argv[])
{
	GtkWidget *window;
	rclk *clock;

	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "RClock");

	g_signal_connect(G_OBJECT(window), "destroy",
			G_CALLBACK(clock_destroy), NULL);

	gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
	gtk_widget_set_app_paintable(window, TRUE);

	gtk_widget_show_all(window);

	/* create the clock */
	clock = clock_init(window);

	g_timeout_add_seconds(1, &clock_tick, clock);
	g_signal_connect(G_OBJECT(window), "expose-event",
			G_CALLBACK(clock_expose), clock);

	gtk_main();

	clock_free(clock);

	return EXIT_SUCCESS;
}

rclk* clock_init(GtkWidget *widget)
{
	int width, height, i;
	double millis = ((double) time(NULL)) * 1000;

	int32_t year = 0;
	int32_t day_of_year = 0;
	int32_t day_of_month = 0;
	int32_t month = 0;
	int32_t hours = 0;
	int32_t minutes = 0;
	int32_t seconds = 0;

	rclk *clock;

	/* alloc memory */
	clock = malloc(sizeof(rclk));

	clock_date_compute(millis, &month,
			&day_of_month, &day_of_year, &year);
	clock_time_compute(millis, &seconds, &minutes, &hours);

	double angles[5] = {
		(2 * M_PI / 60) * seconds,
		(2 * M_PI / 60) * minutes,
		(2 * M_PI / 24) * hours,
		(2 * M_PI / 31) * day_of_month,
		(2 * M_PI / 12) * month};

	double d_angles[5] = {
		(2 * M_PI / 60),
		(2 * M_PI / 3600),
		(2 * M_PI / 86400),
		(2 * M_PI / 2678400),
		(2 * M_PI / 32140800)};

	clock->cr = gdk_cairo_create(widget->window);
	clock->widget = widget;
	clock->started = (int32_t) time(NULL);

	gtk_window_get_size(GTK_WINDOW(widget), &width, &height);

	/* create the various rings */
	for(i = 0; i < 5; ++i) {
		clock->rings[i].radius = 50 + (20 * i);
		clock->rings[i].angle = angles[i];
		clock->rings[i].d_angle = d_angles[i];
	}

	return clock;
}

void clock_free(rclk *clock) {
	free(clock);
}

static void clock_destroy(
		GtkWidget *widget,
		GdkEvent *event,
		gpointer data)
{
	gtk_main_quit();
}

static gboolean clock_expose(
		GtkWidget *widget,
		GdkEvent *event,
		gpointer data)
{
	clock_render((rclk *) data);
	return FALSE;
}

static gboolean clock_tick(gpointer data)
{
	int width, height;
	rclk *clock;

	clock = (rclk *) data;

	gtk_window_get_size(GTK_WINDOW(clock->widget), &width, &height);

	GdkRectangle rect;

	rect.x = 0;
	rect.y = 0;
	rect.width = width;
	rect.height = height;

	gdk_window_invalidate_rect(clock->widget->window, &rect, FALSE);

	return TRUE;
}

void clock_render(rclk *clock)
{
	cairo_t *cr;

	int width, height, i;
	double offset, angle1, angle2;

	double elapsed = (double) (((int32_t) time(NULL)) - clock->started);

	gtk_window_get_size(GTK_WINDOW(clock->widget), &width, &height);

	clock->cr = gdk_cairo_create(clock->widget->window);
	cr = clock->cr;

	offset = M_PI / -2.0;

	for(i = 0; i < 5; ++i) {
		angle2 = fmod(clock->rings[i].angle +
			(clock->rings[i].d_angle * elapsed), 2 * M_PI) + offset;
		angle1 = angle2 + offset;

		cairo_set_source_rgb(cr, 1, 1, 1);
		cairo_new_sub_path(cr);
		cairo_set_line_width(cr, 4.0);
		cairo_arc(cr, width/2, height/2, clock->rings[i].radius, 0,
				2 * M_PI);
		cairo_stroke(cr);

		cairo_set_source_rgba(cr, 0.3, 0.3, 0.3, 0.4);
		cairo_new_sub_path(cr);
		cairo_set_line_width(cr, 2.0);
		cairo_arc(cr, width/2, height/2, clock->rings[i].radius,
				offset, angle2);
		cairo_stroke(cr);
	}

	cairo_destroy(clock->cr);
}
