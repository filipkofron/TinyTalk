#include "BuiltinFile.h"

static bool checkIfExistsAndCloseFile(RefPtr<TTObject> file)
{
    RefPtr<TTObject> fd = file->getField(TO_TT_STR("fd"));
    if(&fd)
    {
        if(fd->getLiteral()->length != sizeof(FILE *))
        {
            std::cerr << "Invalid size of file handle." << std::endl;
            throw std::exception();
        }
        FILE **fileHandle = (FILE **) fd->getLiteral()->data;
        if(*fileHandle)
        {
            fclose(*fileHandle);
            *fileHandle = NULL;
        }
        return true;
    }
    return false;
}

static void createFileHandle(RefPtr<TTObject> file)
{
    RefPtr<TTObject> fd = TTLiteral::createByteArray(sizeof(FILE *));
    FILE **fileHandle = (FILE **) fd->getLiteral()->data;
    *fileHandle = NULL;
    file->addField(TO_TT_STR("fd"), fd);
}

RefPtr<TTObject> BuiltinFileIOOpenPathModeFile::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values)
{
    BUILTIN_CHECK_ARGS_COUNT(3, 3);
    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_STRING(0);
    BUILTIN_CHECK_LITERAL(1);
    BUILTIN_CHECK_STRING(1);

    if (!checkIfExistsAndCloseFile(values[2]))
    {
        createFileHandle(values[2]);
    }

    FILE *file = fopen((const char *) values[0]->getLiteral()->data, (const char *) values[1]->getLiteral()->data);
    RefPtr<TTObject> fd = values[2]->getField(TO_TT_STR("fd"));
    FILE **fileHandle = (FILE **) fd->getLiteral()->data;
    *fileHandle = file;

    return dest;
}

RefPtr<TTObject> BuiltinFileIOClose::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);
    checkIfExistsAndCloseFile(values[0]);
}

