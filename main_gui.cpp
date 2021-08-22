#include <iostream>
#include <gtk/gtk.h>

// our custom headers
#include "DiffFinder.hpp"

int main(int argc, char **argv)
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
    GError          *err = NULL; 

    gtk_init (NULL, NULL);
    
    builder = gtk_builder_new();
    
    if (0 == gtk_builder_add_from_file (builder, "../UI_Design/DiffFinder.glade", &err))
    {
        std::cerr << "err message : " << err->message << std::endl;
        exit(-1);
    }

    if (builder == NULL)
    {
        std::cerr << "Builder is NULL\n";
        exit(-1);
    }

    GObject* gObj = gtk_builder_get_object(builder, "window1");
    
    if (gObj == NULL)
    {
        std::cout << "gObj is NULL\n";
        exit(-1);
    }

    window = GTK_WIDGET( gObj );

    if (window == NULL)
    {
        std::cout << "Window is NULL\n";
        exit(-1);
    }

    gtk_builder_connect_signals (builder, NULL);
    g_object_unref(G_OBJECT (builder));
     
    gtk_widget_show (window);               
    gtk_main ();

    return 0;
}