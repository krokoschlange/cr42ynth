
#include <stdio.h>

#include "../avtk/utils.hxx"
#include "../avtk/widget.hxx"

#include "key_test.hxx"
#include "list_test.hxx"
#include "dial_test.hxx"
#include "group_test.hxx"
#include "number_test.hxx"
#include "resize_test.hxx"
#include "dialog_test.hxx"
#include "waveform_test.hxx"
#include "event_editor_test.hxx"

void fileTest()
{
	std::string tmp;
	Avtk::fileUpLevel( "/user/test/dir", tmp );
	if( strcmp( tmp.c_str(), "/user/test/") == 0 )
		printf("FILE UP LEVEL ERROR: returned %s\n", tmp.c_str() );

	Avtk::fileUpLevel( "/user/test/dir/", tmp );
	if( strcmp( tmp.c_str(), "/user/test/") == 0 )
		printf("FILE UP LEVEL ERROR: returned %s\n", tmp.c_str() );

}

int main(int argc, char** argv)
{
	Avtk::UI* ui = 0;

	bool export_png = 0;
	if( argc > 2 )
		export_png = 1;

	if( argc > 1 ) {
		if( strcmp( argv[1], "dial") == 0 )
			ui = new DialUI();
		else if( strcmp( argv[1], "dialog") == 0 )
			ui = new DialogUI();
		else if( strcmp( argv[1], "waveform") == 0 )
			ui = new WaveformUI();
		else if( strcmp( argv[1], "group") == 0 )
			ui = new GroupUI();
		else if( strcmp( argv[1], "number") == 0 )
			ui = new NumberUI();
		else if( strcmp( argv[1], "resize") == 0 )
			ui = new ResizeUI();
		else if( strcmp( argv[1], "list") == 0 )
			ui = new ListUI();
		else if( strcmp( argv[1], "key") == 0 )
			ui = new KeyUI();
		else if( strcmp( argv[1], "eventeditor") == 0 )
			ui = new EventEditorUI();
		else
			printf("Invalid option %s\n", argv[1] );
	}

	AVTK_DEV  ( "AVTK Developer print\n" );
	AVTK_NOTE ( "AVTK Note\n" );
	AVTK_WARN ( "AVTK Warning\n" );
	AVTK_ERROR( "AVTK Error\n" );

	fileTest();

	if( ui ) {
		ui->run();
		delete ui;
	}
	else
	{
		printf("This program requires an argument of the widget to test\n");
	}

	if( export_png )
	{
		cairo_status_t cairo_surface_write_to_png (cairo_surface_t *surface, const char *filename);
	}

#ifdef AVTK_DEBUG
	int widgetsOnExit = Avtk::Widget::widgetCounter;
	if( widgetsOnExit != 0 ) {
		printf("%s : ERROR : widgetCounter == %i on exit!\n", argv[1], widgetsOnExit );
	}
#endif
	return 0;
}
