//  Main program for SOXMAP
//

#include <qapplication.h>
#include "soxgui.h"
#include <string.h>
//#include "soxmap.h"

int main( int argc, char *argv[] )
{
  QApplication app( argc, argv );

  // Transfer control to the GUI
  Soxgui gui;
  app.setMainWidget( &gui );
  gui.show();
  return app.exec();
}

