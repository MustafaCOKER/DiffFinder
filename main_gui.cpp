#include <iostream>
#include <string>
#include <gtk/gtk.h>

// our custom headers
#include "DiffFinder.hpp"

char *referenceFileName = nullptr,
     *anotherFileName = nullptr;

const char *DEFAULT_VAL_FOR_REFERENCE_FILE_TV = "/Path/Reference/File/..";
const char *DEFAULT_VAL_FOR_ANOTHER_FILE_TV = "/Path/Another/File/..";

GtkTextBuffer *referencePathBuffer = nullptr;
GtkTextIter iterForRef;

GtkTextBuffer *anotherPathBuffer = nullptr;
GtkTextIter iterForAno;

GtkWidget   *window     = nullptr;
DiffFinder *finderInstance = nullptr;

void show_warning(const char * const message)
{
    
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            message);
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_info(const char * const message)
{
    
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            message);
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

static void getReferenceFileName(GtkFileChooser *file_chooser, gpointer data)
{
    GtkWidget *preview;
    
    GdkPixbuf *pixbuf;
    gboolean have_preview;

    preview = GTK_WIDGET (data);
    referenceFileName = gtk_file_chooser_get_preview_filename (file_chooser);

    gtk_text_buffer_set_text(referencePathBuffer, "", 0);

    gtk_text_buffer_get_iter_at_offset(referencePathBuffer, &iterForRef, 0);
    gtk_text_buffer_create_tag(referencePathBuffer, "lmarg", "left_margin", 1, NULL);
    Result res = finderInstance->setReference(referenceFileName);
    
    if (res == Result::Image_CanNOT_Load)
    {
        show_warning("Reference Image Could NOT Loaded Succesful, Please Check the Paths");
        gtk_text_buffer_create_tag(referencePathBuffer, "red_bg", "background", "red", NULL);
        gtk_text_buffer_insert_with_tags_by_name(referencePathBuffer, &iterForRef, DEFAULT_VAL_FOR_REFERENCE_FILE_TV, -1, "lmarg", "red_bg", NULL);

    } else {
        gtk_text_buffer_create_tag(referencePathBuffer, "green_bg", "background", "green", NULL);
        gtk_text_buffer_insert_with_tags_by_name(referencePathBuffer, &iterForRef, referenceFileName, -1, "lmarg", "green_bg", NULL);
    }

}

static void getAnotherFileName(GtkFileChooser *file_chooser, gpointer data)
{
    GtkWidget *preview;
    
    GdkPixbuf *pixbuf;
    gboolean have_preview;

    preview = GTK_WIDGET (data);
    anotherFileName = gtk_file_chooser_get_preview_filename (file_chooser);

    gtk_text_buffer_set_text(anotherPathBuffer, "", 0);

    gtk_text_buffer_get_iter_at_offset(anotherPathBuffer, &iterForAno, 0);
    gtk_text_buffer_create_tag(anotherPathBuffer, "lmarg", "left_margin", 1, NULL);
    gtk_text_buffer_create_tag(anotherPathBuffer, "green_bg", "background", "green", NULL);

    Result res = finderInstance->setAnother(anotherFileName);

    if (res == Result::Image_CanNOT_Load)
    {
        show_warning("Another Image Could NOT Loaded Succesful, Please Check the Paths");
        gtk_text_buffer_create_tag(anotherPathBuffer, "red_bg", "background", "red", NULL);
        gtk_text_buffer_insert_with_tags_by_name(anotherPathBuffer, &iterForAno, DEFAULT_VAL_FOR_ANOTHER_FILE_TV, -1, "lmarg", "red_bg", NULL);
    } else {
        gtk_text_buffer_create_tag(anotherPathBuffer, "green_bg", "background", "green", NULL);
        gtk_text_buffer_insert_with_tags_by_name(anotherPathBuffer, &iterForAno, anotherFileName, -1, "lmarg", "green_bg", NULL);
    }
}

static void destroy(GtkWidget *widget, gpointer *data)
{
    DiffFinder::deleteInstance();

    g_free (referenceFileName);
    g_free (anotherFileName);

    gtk_main_quit();
}

static void run_diffFinder(GtkWidget *widget, gpointer *data)
{
    Result res = finderInstance->isDifferent(nullptr, 0.048);

    switch(res)
    {
    case Result::Image_CanNOT_Load:
        show_warning("Images Could NOT Loaded Succesful, Please Check the Paths");
        break;
    case Result::Different:
        show_info("Images Are Different");
        break;
    case Result::Same:
        show_info("Images Are Almost Same");
        break;
    default:
        std::cerr << "Unknown Situation, Exiting !\n";
        exit(-1); 
    }
}

int main(int argc, char **argv)
{
    GtkBuilder  *builder    = nullptr; 
    GError      *err        = nullptr; 

    // file browsers
    GtkWidget   *fb_ref = nullptr,
                *fb_ano = nullptr;

    // buttons
    GtkWidget   *b_exit = nullptr,
                *b_start = nullptr;

    // textviews
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

    b_start = GTK_WIDGET( gtk_builder_get_object(builder, "b_start"));
    if (b_start == nullptr)
    {
        std::cout << "b_start is nullptr\n";
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
    anotherPathBuffer   = gtk_text_view_get_buffer(GTK_TEXT_VIEW(anotherPath));
    
    gtk_text_buffer_get_iter_at_offset(referencePathBuffer, &iterForRef, 0);
    gtk_text_buffer_get_iter_at_offset(anotherPathBuffer, &iterForAno, 0);
    
    gtk_text_buffer_create_tag(referencePathBuffer, "lmarg", "left_margin", 1, NULL);
    gtk_text_buffer_create_tag(anotherPathBuffer,   "lmarg", "left_margin", 1, NULL);

    gtk_text_buffer_create_tag(referencePathBuffer, "red_bg", "background", "red", NULL);
    gtk_text_buffer_create_tag(anotherPathBuffer,   "red_bg", "background", "red", NULL);

    gtk_text_buffer_insert_with_tags_by_name(referencePathBuffer, &iterForRef, DEFAULT_VAL_FOR_REFERENCE_FILE_TV, -1, "lmarg", "red_bg", NULL);
    gtk_text_buffer_insert_with_tags_by_name(anotherPathBuffer, &iterForAno, DEFAULT_VAL_FOR_ANOTHER_FILE_TV, -1, "lmarg", "red_bg", NULL);

    g_signal_connect(G_OBJECT(fb_ref), "selection-changed", G_CALLBACK(getReferenceFileName), nullptr);
    g_signal_connect(G_OBJECT(fb_ano), "selection-changed", G_CALLBACK(getAnotherFileName), nullptr);

    g_signal_connect(G_OBJECT(b_exit), "clicked", G_CALLBACK(destroy), nullptr);
    g_signal_connect(G_OBJECT(b_start), "clicked", G_CALLBACK(run_diffFinder), nullptr);

    gtk_builder_connect_signals (builder, nullptr);
    g_object_unref(G_OBJECT (builder));

    finderInstance = DiffFinder::getInstance();

    gtk_widget_show(window);               
    gtk_main();

    return 0;
}