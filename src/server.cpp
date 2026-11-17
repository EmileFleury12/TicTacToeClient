#include "entrypoint.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <_socket_types.h>

typedef int SOCKET;

#pragma comment(lib, "Ws2_32.lib")
int iResult;

int raylib_start(void)
{

#define DEFAULT_PORT "80"

  struct addrinfo *result = NULL, *ptr = NULL, hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
  if (iResult != 0)
  {
    printf("getaddrinfo failed: %d\n", iResult);
  }

  SOCKET ListenSocket;

  ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

  if (ListenSocket == -1)
  {
    printf("Error at socket(): %ld\n");
    freeaddrinfo(result);
  }

  iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
  if (iResult == -1)
  {
    printf("bind failed with error: %d\n");
    freeaddrinfo(result);
  }

  freeaddrinfo(result);

  if (listen(ListenSocket, SOMAXCONN) == -1)
  {
    printf("Listen failed with error: %ld\n");
  }

  SOCKET ClientSocket;

  ClientSocket = INVALID_SOCKET;

  // Accepter un socket client
  ClientSocket = accept(ListenSocket, NULL, NULL);
  if (ClientSocket == INVALID_SOCKET)
  {
    printf("accept failed: %d\n");
  }

#define DEFAULT_BUFLEN 512

  char recvbuf[DEFAULT_BUFLEN];

  int taille = recv(ClientSocket, recvbuf, sizeof(recvbuf) - 1, 0);

  if (taille > 0)
  {
    recvbuf[taille] = '\0';

    std::string requete = recvbuf;

    if (requete.find("GET ") == 0)
    {
      std::cout << "Methode GET recue\n";
      std::string reponse =
          "HTTP/1.1 200 OK\r\n"
          "Content-Length: 5\r\n"
          "Connection: close\r\n"
          "\r\n"
          "Salut";

      send(ClientSocket, reponse.c_str(),(int)reponse.length(), 0);
      std::cout << "Reponse HTTP envoyee au client\n";
    }

    else if (requete.find("HEAD ") == 0)
      std::cout
          << "Methode HEAD recue\n";

    else if (requete.find("PUT ") == 0)
      std::cout
          << "Methode PUT recue\n";

    else if (requete.find("DELETE ") == 0)
      std::cout
          << "Methode DELETE recue\n";

    else
      std::cout << "Methode inconnue\n";
  }

  /*int iSendResult;
  int recvbuflen = DEFAULT_BUFLEN;

  // Recevoir jusqu’à ce que le correspondant ferme la connexion
  do
  {
    iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0)
    {
      printf("Bytes received: %d\n", iResult);

      // Renvoyer le buffer à l'expéditeur
      iSendResult = send(ClientSocket, recvbuf, iResult, 0);
      if (iSendResult == SOCKET_ERROR)
      {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
      }
      printf("Bytes sent: %d\n", iSendResult);
    }
    else if (iResult == 0)
      printf("Connection closing...\n");
    else
    {
      printf("recv failed: %d\n", WSAGetLastError());
      closesocket(ClientSocket);
      WSACleanup();
    }

  } while (iResult > 0);*/
}

int main()
{
  raylib_start();

  return 0;
}