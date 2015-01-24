#include "BuiltinFile.h"
#include "Runtime.h"

bool checkIfExistsAndCloseFile(RefPtr<TTObject> file)
{
    RefPtr<TTObject> fd = file->getField(TO_TT_STR("fd"));
    if(&fd)
    {
        if(fd->getLiteral()->length != sizeof(FILE *))
        {
            std::cerr << "[Builtin] File: Invalid size of file handle." << std::endl;
            KILL;
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

void createFileHandle(RefPtr<TTObject> file)
{
    RefPtr<TTObject> fd = TTLiteral::createByteArray(sizeof(FILE *));
    FILE **fileHandle = (FILE **) fd->getLiteral()->data;
    *fileHandle = NULL;
    file->addField(TO_TT_STR("fd"), fd);
}

FILE **checkIfExistsAndIsOpenedThenReturnFD(RefPtr<TTObject> file)
{
    RefPtr<TTObject> fd = file->getField(TO_TT_STR("fd"));
    if (!&fd)
    {
        std::cerr << "[Builtin] File: file handle missing!" << std::endl;
        KILL;
    }

    if (fd->getLiteral()->length != sizeof(FILE *))
    {
        std::cerr << "[Builtin] File: Invalid size of file handle." << std::endl;
        KILL;
    }
    FILE **fileHandle = (FILE **) fd->getLiteral()->data;
    if (!*fileHandle)
    {
        std::cerr << "[Builtin] File: not opened!" << std::endl;
        KILL;
    }
    return fileHandle;
}

RefPtr<TTObject> BuiltinFileIOOpenModeFile::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
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

RefPtr<TTObject> BuiltinFileIOClose::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);
    checkIfExistsAndCloseFile(values[0]);

    return dest;
}

RefPtr<TTObject> BuiltinFileIORead::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);
    FILE *fd = *checkIfExistsAndIsOpenedThenReturnFD(values[0]);
    Runtime::gcBarrier.enteringBlocking();
    int val = fgetc(fd);
    Runtime::gcBarrier.leavingBlocking();
    RefPtr<TTObject> res = TTLiteral::createIntegerLiteral(val);
    return res;
}

RefPtr<TTObject> BuiltinFileIOWriteFile::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(2, 2);
    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_INTEGER(0);
    FILE *fd = *checkIfExistsAndIsOpenedThenReturnFD(values[1]);
    int val = *(int32_t *) values[0]->getLiteral()->data;
    Runtime::gcBarrier.enteringBlocking();
    int ret = fputc(val, fd);
    Runtime::gcBarrier.leavingBlocking();
    RefPtr<TTObject> written;

    if(ret != -1)
    {
        written = Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }
    else
    {
        written = Runtime::globalEnvironment->getField(TO_TT_STR("False"));
    }

    return written;
}

RefPtr<TTObject> BuiltinFileIOWriteStringFile::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(2, 2);
    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_STRING(0);

    FILE *fd = *checkIfExistsAndIsOpenedThenReturnFD(values[1]);

    uint8_t *temp = (uint8_t *) malloc(values[0]->getLiteral()->length);
    memcpy(temp, values[0]->getLiteral()->data, values[0]->getLiteral()->length);
    Runtime::gcBarrier.enteringBlocking();
    int ret = fputs((const char *) temp, fd);
    free(temp);
    Runtime::gcBarrier.leavingBlocking();
    RefPtr<TTObject> written;

    if(ret != -1)
    {
        written = Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }
    else
    {
        written = Runtime::globalEnvironment->getField(TO_TT_STR("False"));
    }

    return written;
}

RefPtr<TTObject> BuiltinFileIOIsOK::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);
    RefPtr<TTObject> fd = values[0]->getField(TO_TT_STR("fd"));
    if (!&fd)
    {
        std::cerr << "[Builtin] File: file handle missing!" << std::endl;
        KILL;
    }

    if (fd->getLiteral()->length != sizeof(FILE *))
    {
        std::cerr << "[Builtin] File: Invalid size of file handle." << std::endl;
        KILL;
    }
    if(*(FILE **) fd->getLiteral()->data == NULL)
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
    }
    return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
}

RefPtr<TTObject> BuiltinFileIOClearErr::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);
    RefPtr<TTObject> fd = values[0]->getField(TO_TT_STR("fd"));
    if (!&fd)
    {
        std::cerr << "[Builtin] File: file handle missing!" << std::endl;
        KILL;
    }

    if (fd->getLiteral()->length != sizeof(FILE *))
    {
        std::cerr << "[Builtin] File: Invalid size of file handle." << std::endl;
        KILL;
    }

    rewind( *(FILE **) fd->getLiteral()->data );

    return dest;
}

#define LINE_LENGTH_MAX (1024 * 1024 + 1)

RefPtr<TTObject> BuiltinFileIOReadLine::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);
    FILE *fd = *checkIfExistsAndIsOpenedThenReturnFD(values[0]);

    Runtime::gcBarrier.enteringBlocking();
    uint32_t size = 1;
    char *buffer = (char *) malloc(size + 1);
    uint32_t i = 0;
    do
    {
        int c = fgetc(fd);
        if(c == -1)
        {
            RefPtr<TTObject> t = Runtime::globalEnvironment->getField(TO_TT_STR("True"));
            values[0]->setField(TO_TT_STR("eof"), t);
            break;
        }
        if(c == '\n' || c == '\r')
        {
            break;
        }
        if(i == size)
        {
            size *= 2;
            buffer = (char *) realloc(buffer, size + 1);
        }
        buffer[i++] = (char) c;
    }
    while(i < LINE_LENGTH_MAX);

    buffer[i] = '\0';

    Runtime::gcBarrier.leavingBlocking();
    RefPtr<TTObject> res = TTLiteral::createStringLiteral(TO_TT_STR(buffer));
    free(buffer);
    return res;
}
