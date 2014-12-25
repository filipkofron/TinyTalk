#include "BuiltinSocket.h"
#include "BuiltinFile.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void checkServerSocketMissing(RefPtr<TTObject> socket)
{
    RefPtr<TTObject> socketFd = socket->getField(TO_TT_STR("socketFd"));
    RefPtr<TTObject> socketAddr = socket->getField(TO_TT_STR("socketAddr"));

    if(!&socketFd)
    {
        std::cerr << "[Buitlin]: Server socket: Missing socket descriptor" << std::endl;
        throw std::exception();
    }

    if(!&socketAddr)
    {
        std::cerr << "[Buitlin]: Server socket: Missing socket address struct" << std::endl;
        throw std::exception();
    }
}

void checkServerSocketOpened(RefPtr<TTObject> socket)
{
    RefPtr<TTObject> socketFd = socket->getField(TO_TT_STR("socketFd"));

    if(*(int *) socketFd->getLiteral()->data < 0)
    {
        std::cerr << "[Buitlin]: Server socket: Not opened." << std::endl;
        throw std::exception();
    }
}

void closeServerSocketIfOpened(RefPtr<TTObject> socket)
{
    RefPtr<TTObject> socketFd = socket->getField(TO_TT_STR("socketFd"));

    if(&socketFd)
    {
        int sfd = *(int *) socketFd->getLiteral()->data;
        if(sfd >= 0)
        {
            close(sfd);
            *(int *) socketFd->getLiteral()->data = -1;
        }
    }
}

RefPtr<TTObject> BuiltinSocketCreateTCPServerSocketAddressPortFile::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values)
{
    BUILTIN_CHECK_ARGS_COUNT(3, 3);

    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_STRING(0);
    BUILTIN_CHECK_LITERAL(1);
    BUILTIN_CHECK_INTEGER(1);

    /* TODO: Check ip and port values!! */

    closeServerSocketIfOpened(values[2]);

    struct sockaddr_in serv_addr;
    int sfd = 0;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        std::cerr << "[Buitlin]: Server socket: ERROR opening socket" << std::endl;
        throw std::exception();
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr((const char *)values[0]->getLiteral()->data);
    serv_addr.sin_port = htons((uint16_t) *(int32_t *) values[1]->getLiteral()->data);

    if (bind(sfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0)
    {
        std::cerr << "[Buitlin]: Server socket: ERROR binding socket" << std::endl;
        throw std::exception();
    }

    listen(sfd,5);

    RefPtr<TTObject> socketFd = TTLiteral::createByteArray(sizeof(int));
    RefPtr<TTObject> socketAddr = TTLiteral::createByteArray(sizeof(serv_addr));

    *(int *) socketFd->getLiteral()->data = sfd;
    *(struct sockaddr_in *) socketAddr->getLiteral()->data = serv_addr;

    values[2]->addField(TO_TT_STR("socketFd"), socketFd);
    values[2]->addField(TO_TT_STR("socketAddr"), socketAddr);
/*    createFileHandle(values[2]);

    RefPtr<TTObject> fd = values[2]->getField(TO_TT_STR("fd"));
    FILE **fileHandle = (FILE **) fd->getLiteral()->data;
    *fileHandle = fdopen(sfd, "wb+");
*/
    return dest;
}

RefPtr<TTObject> BuiltinSocketCloseTCPServerSocketFile::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    checkServerSocketMissing(values[0]);
    checkServerSocketOpened(values[0]);
    closeServerSocketIfOpened(values[0]);

    return dest;
}

RefPtr<TTObject> BuiltinSocketTCPServerSocketAcceptClientFile::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values)
{
    BUILTIN_CHECK_ARGS_COUNT(2, 2);

    checkServerSocketMissing(values[0]);
    checkServerSocketOpened(values[0]);

    closeServerSocketIfOpened(values[1]);

    sockaddr_in clientAddr;
    socklen_t clientAddrSize = (socklen_t) sizeof(sockaddr_in);

    RefPtr<TTObject> socketFd = values[0]->getField(TO_TT_STR("socketFd"));
    int listenSd = *(int *) socketFd->getLiteral()->data;

    int clientSd = 0;

    do
    {
        clientSd = accept(listenSd, (sockaddr *) &clientAddr, &clientAddrSize);
    } while(clientSd < 0);

    RefPtr<TTObject> clientSocketFd = TTLiteral::createByteArray(sizeof(int));
    RefPtr<TTObject> clientSocketAddr = TTLiteral::createByteArray(sizeof(clientAddr));

    *(int *) clientSocketFd->getLiteral()->data = clientSd;
    *(struct sockaddr_in *) clientSocketAddr->getLiteral()->data = clientAddr;

    values[1]->addField(TO_TT_STR("socketFd"), clientSocketFd);
    values[1]->addField(TO_TT_STR("socketAddr"), clientSocketAddr);
    createFileHandle(values[1]);

    RefPtr<TTObject> fd = values[1]->getField(TO_TT_STR("fd"));
    FILE **fileHandle = (FILE **) fd->getLiteral()->data;
    *fileHandle = fdopen(clientSd, "wb+");

    return dest;
}

RefPtr<TTObject> BuiltinSocketCloseTCPClientSocketFile::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    checkServerSocketMissing(values[0]);
    checkServerSocketOpened(values[0]);
    closeServerSocketIfOpened(values[0]);
    checkIfExistsAndCloseFile(values[0]);
    return dest;
}
