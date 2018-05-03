#include <gtk/gtk.h>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;
const gchar* stylefile = "style.css";

GtkBuilder      *builder; 

extern "C" void test(GtkWidget *button, gpointer user_data)
{
    cout << "Test" << endl;
}

gchar *get_file( const gchar * filename)
{
    string temp = "";
    string read_file = "";
    gchar * ret;

    fstream my_file(filename);
    while( getline(my_file, temp) )
    {
        read_file += temp;
        read_file += '\n';
    }
    ret = (gchar*)malloc(sizeof(gchar)*(read_file.length()+1));
    strcpy(ret, read_file.c_str());
    return ret;
}
void write_file( const gchar * filename, gchar * text )
{
    ofstream my_file(filename);
    my_file << text;
    my_file.close();
}
void refresh_css()
{
    GtkCssProvider *provider;
    GdkScreen *screen;
    GdkDisplay *display;

    /* Apply CSS */
    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GError *error = 0;
    gtk_css_provider_load_from_file(provider, g_file_new_for_path(stylefile), &error);
    g_object_unref(provider);
    /**/
}
extern "C" void reload_click(GtkWidget *button, gpointer user_data)
{
    GtkTextView     *textbox;
    GtkTextBuffer   *buffer;

    /* Get Widgets */
    textbox = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "textbox"));
    buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "file_buffer"));

    refresh_css();

    /*Read textfile*/
    gchar *file_contents = get_file(stylefile);
    gtk_text_buffer_set_text(buffer, file_contents, strlen(file_contents)); 
    gtk_text_view_set_buffer(textbox, buffer);
    g_free(file_contents);
    /**/
}
extern "C" void write_click(GtkWidget *button, gpointer user_data)
{
    GtkTextView     *textbox;
    GtkTextBuffer   *buffer;

    /*File */
    textbox = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "textbox"));
    buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "file_buffer"));

    gchar * text_contents = NULL;
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds (buffer, &start, &end);
    text_contents = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
    
    write_file(stylefile, text_contents);
    g_free(text_contents);

    refresh_css();
}

int main(int argc, char *argv[])
{
    GtkWidget       *window;
    GtkTextView     *textbox;
    GtkTextBuffer   *buffer;

    /* CSS Variables */
    GtkCssProvider *provider;
    GdkScreen *screen;
    GdkDisplay *display;
    gint len = 9;
    /* End CSS Variables */

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "interface.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect (window, "delete_event", gtk_main_quit, NULL);

    textbox = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "textbox"));
    buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "file_buffer"));

    gchar *file_contents = get_file(stylefile);
    gtk_text_buffer_set_text(buffer, file_contents, strlen(file_contents)); 
    gtk_text_view_set_buffer(textbox, buffer);
    g_free(file_contents);

    gtk_builder_connect_signals(builder, NULL);

    /* Load CSS */
    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GError *error = 0;
    gtk_css_provider_load_from_file(provider, g_file_new_for_path(stylefile), &error);
    g_object_unref(provider);
    /* End Load CSS */

    //g_object_unref(builder);

    gtk_widget_show_all(window);                
    gtk_main();

    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
