
#include "mt_filechooser.hpp"

#include <initializer_list>
#include <sstream>
#include <vector>

void TOOLKIT_NAMESPACE::FileChooser::catchReturn()
{
    switch (CommDlgExtendedError())
    {
    case CDERR_DIALOGFAILURE:
        Error("CDERR_DIALOGFAILURE");
        break;
    case CDERR_FINDRESFAILURE:
        Error("CDERR_FINDRESFAILURE");
        break;
    case CDERR_INITIALIZATION:
        Error("CDERR_INITIALIZATION");
        break;
    case CDERR_LOADRESFAILURE:
        Error("CDERR_LOADRESFAILURE");
        break;
    case CDERR_LOADSTRFAILURE:
        Error("CDERR_LOADSTRFAILURE");
        break;
    case CDERR_LOCKRESFAILURE:
        Error("CDERR_LOCKRESFAILURE");
        break;
    case CDERR_MEMALLOCFAILURE:
        Error("CDERR_MEMALLOCFAILURE");
        break;
    case CDERR_MEMLOCKFAILURE:
        Error("CDERR_MEMLOCKFAILURE");
        break;
    case CDERR_NOHINSTANCE:
        Error("CDERR_NOHINSTANCE");
        break;
    case CDERR_NOHOOK:
        Error("CDERR_NOHOOK");
        break;
    case CDERR_NOTEMPLATE:
        Error("CDERR_NOTEMPLATE");
        break;
    case CDERR_STRUCTSIZE:
        Error("CDERR_STRUCTSIZE");
        break;
    case FNERR_BUFFERTOOSMALL:
        Error("FNERR_BUFFERTOOSMALL");
        break;
    case FNERR_INVALIDFILENAME:
        Error("FNERR_INVALIDFILENAME");
        break;
    case FNERR_SUBCLASSFAILURE:
        Error("FNERR_SUBCLASSFAILURE");
        break;
    default:
        Debug("User cancelled.");
    }
}
TOOLKIT_NAMESPACE::FileChooser::FileChooser()
{
    ZeroMemory(&chooser, sizeof(OPENFILENAME));
    ZeroMemory(&filename, sizeof(filename));

    chooser.lStructSize = sizeof(OPENFILENAME);
    chooser.hwndOwner = NULL;
    chooser.lpstrFile = filename;
    chooser.nMaxFile = MAX_PATH;
    chooser.lpstrTitle = "Selecione um arquivo";
    chooser.Flags = DONT_ADD_TO_RECENT | PATH_MUST_EXIST;
}
void TOOLKIT_NAMESPACE::FileChooser::setTitle(const char *title)
{
    chooser.lpstrTitle = title;
}
void TOOLKIT_NAMESPACE::FileChooser::setFlags(fileChooserFlags flags)
{
    chooser.Flags = flags;
}
void TOOLKIT_NAMESPACE::FileChooser::setFilter(std::initializer_list<std::pair<const char *, std::initializer_list<const char *>>> filters)
{
    std::stringstream stream;
    for (auto &filter : filters)
    {
        stream << filter.first;
        stream.write("\0", 1);
        for (auto it = filter.second.begin(); it != filter.second.end(); ++it)
        {
            stream << *it;
            if (it != filter.second.end())
                stream << ";";
        }
        stream.write("\0", 1);
    }
    chooser.lpstrFilter = stream.str().c_str();
}

int TOOLKIT_NAMESPACE::FileChooser::showOpenDialog()
{
    errorlevel = GetOpenFileNameA(&chooser);
    if (errorlevel)
    {
        Debug("Choosed file \"" << filename << "\" (" << errorlevel << ")");
    }
    else
    {
        catchReturn();
    }
    return errorlevel;
}
int TOOLKIT_NAMESPACE::FileChooser::showSaveDialog()
{
    errorlevel = GetSaveFileNameA(&chooser);
    if (errorlevel)
    {
        Debug("Choosed file \"" << filename << "\" (" << errorlevel << ")");
    }
    else
    {
        catchReturn();
    }
    return errorlevel;
}