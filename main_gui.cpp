#include <iostream>
#include <gtk/gtk.h>

// our custom headers
#include "DiffFinder.hpp"

char *referenceFileName = nullptr,
     *anotherFileName = nullptr;

GtkTextBuffer *referencePathBuffer = nullptr;
GtkTextIter iterForRef;

GtkTextBuffer *anotherPathBuffer = nullptr;
GtkTextIter iterForAno;

static void getReferenceFileName(GtkFileChooser *file_chooser, gpointer data)
{
    GtkWidget *preview;
    
    GdkPixbuf *pixbuf;
    gboolean have_preview;

    preview = GTK_WIDGET (data);
    referenceFileName = gtk_file_chooser_get_preview_filename (file_chooser);

    gtk_text_buffer_insert(referencePathBuffer, &iterForRef, referenceFileName, -1);
    
    std::cout << referenceFileName << std::endl;
}

static void getAnotherFileName(GtkFileChooser *file_chooser, gpointer data)
{
    GtkWidget *preview;
    
    GdkPixbuf *pixbuf;
    gboolean have_preview;

    preview = GTK_WIDGET (data);
    anotherFileName = gtk_file_chooser_get_preview_filename (file_chooser);

    gtk_text_buffer_insert(anotherPathBuffer, &iterForAno, anotherFileName, -1);
    std::cout << anotherFileName << std::endl;
}

static void destroy(GtkWidget *widget, gpointer *data)
{
    g_free (referenceFileName);
    g_free (anotherFileName);

    gtk_main_quit();
}

int main(int argc, char **argv)
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
    GError          *err = nullptr; 

    GtkWidget   *fb_ref = nullptr,
                *fb_ano = nullptr;

    GtkWidget *b_exit = nullptr;

    GtkWidget   *referencePath = nullptr,
                *anotherPath = nullptr;

    gtk_init (nullptr, nullptr);

    builder = gtk_builder_new();

    if (0 == gtk_builder_add_from_file (builder, "../UI_Design/DiffFinder.glade", &err))
    {
        std::cerr << "err message : " << err->message << std::endl;
        exit(-1);
    }

    if (builder == nullptr)
    {
        std::cerr << "Builder is nullptr\n";
        exit(-1);
    }

    GObject* gObj = gtk_builder_get_object(builder, "window1");

    if (gObj == nullptr)
    {
        std::cout << "gObj is nullptr\n";
        exit(-1);
    }

    window = GTK_WIDGET( gObj );

    if (window == nullptr)
    {
        std::cout << "Window is nullptr\n";
        exit(-1);
    }

    fb_ref = GTK_WIDGET( gtk_builder_get_object(builder, "referenceFileChooser"));
    if (fb_ref == nullptr)
    {
        std::cout << "fb_ref is nullptr\n";
        exit(-1);
    }

    fb_ano = GTK_WIDGET( gtk_builder_get_object(builder, "anotherFileChooser"));
    if (fb_ano == nullptr)
    {
        std::cout << "fb_ano is nullptr\n";
        exit(-1);
    }

    b_exit = GTK_WIDGET( gtk_builder_get_object(builder, "b_exit"));
    if (b_exit == nullptr)
    {
        std::cout << "b_exit is nullptr\n";
        exit(-1);
    }

    referencePath = GTK_WIDGET( gtk_builder_get_object(builder, "tv_reference"));
    if (referencePath == nullptr)
    {
        std::cout << "tv_reference is nullptr\n";
        exit(-1);
    }

    anotherPath = GTK_WIDGET( gtk_builder_get_object(builder, "tv_another"));
    if (anotherPath == nullptr)
    {
        std::cout << "tv_another is nullptr\n";
        exit(-1);
    }

    referencePathBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(referencePath));
    gtk_text_buffer_get_iter_at_offset(referencePathBuffer, &iterForRef, 0);

    anotherPathBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(anotherPath));
    gtk_text_buffer_get_iter_at_offset(anotherPathBuffer, &iterForAno, 0);
    
    gtk_text_buffer_insert(referencePathBuffer, &iterForRef, "/Path/Reference/File/..", -1);
    gtk_text_buffer_insert(anotherPathBuffer, &iterForAno, "/Path/Another/File/..", -1);

    g_signal_connect(G_OBJECT(fb_ref), "selection-changed", G_CALLBACK(getReferenceFileName), nullptr);
    g_signal_connect(G_OBJECT(fb_ano), "selection-changed", G_CALLBACK(getAnotherFileName), nullptr);

    g_signal_connect(G_OBJECT(b_exit), "clicked", G_CALLBACK(destroy), nullptr);

    gtk_builder_connect_signals (builder, nullptr);
    g_object_unref(G_OBJECT (builder));

    gtk_widget_show(window);               
    gtk_main();

    return 0;
}