// $Id: pathdialog.h,v 1.3 2004/08/07 13:38:07 sonntag Exp $
// Pathdialog class definition
//

#ifndef PATHDIALOG_H
#define PATHDIALOG_H

#include <qdialog.h>
#include <qstring.h>

class Pathdialog : public QDialog
{

  Q_OBJECT

  public:
    Pathdialog(QString logfile,QWidget *parent,
               const char *name=0);  // Constructor
    ~Pathdialog();  // Destructor

  private slots:
    void slotAppend();
    void slotOverwrite();

};

#endif // PATHDIALOG_H
