#include "concesionario.h"
#include <gtk/gtk.h>

struct concesionario *con;
int id;
int selected;

static void funcion_delete(GtkWidget *widget, gpointer data)
{
	curso_concesionario_attr_unset_coche(con, selected);
	gtk_combo_box_text_remove((GtkComboBoxText *)data, selected);
	gtk_combo_box_set_active(GTK_COMBO_BOX(data), 0);
	selected = 0;
}


void combo_selected(GtkWidget *widget, gpointer window)
{ 
	selected = gtk_combo_box_get_active((GtkComboBox *)widget);
}


static void show_delete_window(GtkButton *widget)
{
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *box;
	GtkWidget *combo;
	

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	box = gtk_vbox_new(FALSE, 2);
	combo = gtk_combo_box_text_new();


	gtk_window_set_title((GtkWindow *)window, "Delete coche");
	gtk_window_set_default_size(GTK_WINDOW(window), 250, 250);
	button = gtk_button_new_with_label("Delete coche");


	int i;
	struct coche *c;
	c = curso_coche_alloc();
	int numCoches = curso_concesionario_attr_get_u32(con,
			CURSO_CONCESIONARIO_ATTR_NUM_COCHES);
	char infoCoche[4000];
	for (i = 0; i < numCoches; i++) {
		c = curso_concesionario_attr_get_coche(con,
		CURSO_CONCESIONARIO_ATTR_COCHE, i);

		curso_coche_snprintf(infoCoche, sizeof(infoCoche), c);
		gtk_combo_box_text_append_text((GtkComboBoxText *)combo,
			(gchar *)infoCoche);
	}

	gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
	selected = 0;
	gtk_widget_set_size_request(combo, -1, 50);
	gtk_widget_set_size_request(button, -1, 100);


	gtk_box_pack_start((GtkBox *)box, combo, TRUE, TRUE, 2);
	gtk_box_pack_start((GtkBox *)box, button, TRUE, TRUE, 2);

	gtk_container_add((GtkContainer *)window, box);
	gtk_widget_show_all(window);

	g_signal_connect((GObject *)combo, "changed",
		(GCallback)combo_selected, (gpointer) combo);
	g_signal_connect((GObject *)button, "clicked",
		(GCallback)funcion_delete, (gpointer) combo);

}


static void show_car(GtkButton *widget)
{
	char info[4000];
	curso_concesionario_snprintf(info, sizeof(info), con);
	printf("%s", info);
}


static void add_car(GtkButton *widget, gpointer data)
{

	GtkWidget **entry;
	struct coche *c;
	GtkWidget *entry_ptr_marca;
	GtkWidget *entry_ptr_matricula;
	const gchar *matricula, *marca;

	entry = (GtkWidget **)data;
	entry_ptr_marca = entry[0];
	entry_ptr_matricula = entry[1];


	marca = gtk_entry_get_text((GtkEntry *)entry_ptr_marca);
	matricula = gtk_entry_get_text((GtkEntry *)entry_ptr_matricula);

	c = curso_coche_alloc();
	if (c == NULL)
		return;

	curso_coche_attr_set_u32(c, CURSO_COCHE_ATTR_ID, id++);
	curso_coche_attr_set_str(c, CURSO_COCHE_ATTR_MATRICULA, matricula);
	curso_coche_attr_set_str(c, CURSO_COCHE_ATTR_MARCA, marca);

	curso_concesionario_attr_set_coche(con, CURSO_CONCESIONARIO_ATTR_COCHE,
					   c);


}

static void show_add_window(GtkButton *widget, gpointer data)
{
	GtkWidget *window;
	GtkWidget *label1, *label2;
	GtkWidget *button1, *button2;
	GtkWidget *entry1, *entry2;
	GtkWidget *box1, *box2;
	GtkWidget **entry;

	entry = calloc(1, sizeof(GtkWidget));

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title((GtkWindow *)window, "Add window");
	button1 = gtk_button_new_with_label("Add me");
	button2 = gtk_button_new_with_label("Mostrar por pantalla");
	box1 = gtk_vbox_new(TRUE,2);
	box2 = gtk_hbox_new(TRUE,2);
	label1 = gtk_label_new("Marca:");
	label2 = gtk_label_new("Matricula:");
	entry1 = gtk_entry_new();
	entry2 = gtk_entry_new();

	entry[0] = entry1;
	entry[1] = entry2;

	gtk_box_pack_start ((GtkBox *)box1, label1, TRUE, TRUE, 2);
	gtk_box_pack_start ((GtkBox *)box1, entry1, TRUE, TRUE, 2);
	gtk_box_pack_start ((GtkBox *)box1, label2, TRUE, TRUE, 2);
	gtk_box_pack_start ((GtkBox *)box1, entry2, TRUE, TRUE, 2);

	gtk_box_pack_start ((GtkBox *)box2, button1, TRUE, TRUE, 2);
	gtk_box_pack_start ((GtkBox *)box2, button2, TRUE, TRUE, 2);

	gtk_container_add((GtkContainer *)window, box1);
	gtk_container_add((GtkContainer *)box1, box2);

	g_signal_connect((GObject *)button1, "clicked", (GCallback)add_car,
			 (gpointer)entry);

	g_signal_connect((GObject *)button2, "clicked", (GCallback)show_car,
			 NULL);

	gtk_widget_show_all(window);
}



static void show_select_window(GtkButton *widget, gpointer data)
{

	GtkWidget *window;
	GtkWidget *button1, *button2;
	GtkWidget *box;
	GtkWidget **entry = (GtkWidget **)data;
	GtkWidget *entry_ptr_dueno = entry[0];
	const gchar *dueno;

	dueno = gtk_entry_get_text((GtkEntry *)entry_ptr_dueno);


	curso_concesionario_attr_set_str(con, CURSO_CONCESIONARIO_ATTR_DUENO,
					 dueno);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title((GtkWindow *)window, "Select Window");
	button1 = gtk_button_new_with_label("Add Coche");
	button2 = gtk_button_new_with_label("delete Coche");
	box = gtk_hbox_new(TRUE,2);


	gtk_box_pack_start ((GtkBox *)box, button1, TRUE, TRUE, 2);
	gtk_box_pack_start ((GtkBox *)box, button2, TRUE, TRUE, 2);

	gtk_container_add((GtkContainer *)window, box);

	g_signal_connect((GObject *)button1, "clicked", (GCallback)show_add_window,
			 NULL);
	g_signal_connect((GObject *)button2, "clicked", (GCallback)show_delete_window,
			 NULL);

	printf("Dueno: %s\n", dueno);

	gtk_widget_show_all(window);
}

static void funcion_quit(GtkButton *boton, gpointer data)
{
	curso_concesionario_free(con);
	gtk_main_quit();
}

int main (int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *entry1;
	GtkWidget **entry;
	GtkWidget *vbox;

	con = curso_concesionario_alloc();
	if (con == NULL)
		return -1;


	gtk_init(&argc, &argv);


	entry = calloc(1, sizeof(GtkWidget));

	entry1 = gtk_entry_new();

	entry[0] = entry1;




	

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	button = gtk_button_new_with_label("Hola, ¿Como te llamas?");
	vbox = gtk_vbox_new(TRUE,5);
	gtk_box_pack_start ((GtkBox *)vbox, entry1, TRUE, TRUE, 2);

	gtk_window_set_title((GtkWindow *)window, "Concesionario");

	gtk_box_pack_start((GtkBox *)vbox, button, TRUE, TRUE, 2);

	gtk_container_add((GtkContainer *)window, vbox);

	g_signal_connect((GObject *)button, "clicked",
			 (GCallback)show_select_window, (gpointer)entry);

	g_signal_connect((GObject *)window, "delete-event",
			 (GCallback)funcion_quit, NULL);

	gtk_widget_show_all(window);

	gtk_main();
	return 0;
}
