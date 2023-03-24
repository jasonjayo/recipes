#include "missingfileexception.h"

#include <QMessageBox>

MissingFileException::MissingFileException(QString fileName) : fileName(fileName)
{
    QMessageBox alert;
    alert.setIcon(QMessageBox::Critical);
    alert.setText("Unable to find " + fileName  + " file. File should be in same directory as executable.");
    alert.setWindowTitle(fileName + " file missing");
    alert.exec();
}

const char* MissingFileException::what() const noexcept {
    return "Unable to find file. File should be in same directory as executable.";
}
