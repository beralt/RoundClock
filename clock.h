typedef struct {
	int radius;
	double angle;
	double d_angle;
} rclk_ring;

typedef struct {
	cairo_t *cr;
	GtkWidget *widget;
	int32_t started;
	rclk_ring rings[5];
} rclk;

rclk* clock_init(GtkWidget *widget);
void clock_free(rclk *clock);

static void clock_destroy(
		GtkWidget *widget,
		GdkEvent *event,
		gpointer data);

static gboolean clock_expose(
		GtkWidget *widget,
		GdkEvent *event,
		gpointer data);

static gboolean clock_tick(gpointer data);
void clock_render(rclk *clock);
