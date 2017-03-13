#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

void get_fortune_cookie(char *argv)
{
  int sockfd, newsockfd, ser_len, temp,cookie_num;
  struct sockaddr_in server_addr;
  char str[1];
  char message[2048];

  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      printf("Could Not Open Socket");
      exit(0);
    }


  bzero((char *) &server_addr, sizeof(server_addr));
  bzero((char *) &message, sizeof(message));

  server_addr.sin_family = AF_INET;
  //server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 
  inet_aton(argv, &server_addr.sin_addr.s_addr);
  //  server_addr.sin_port = htons(7891);
  server_addr.sin_port = htons(7070);

  ser_len = sizeof(server_addr);
 temp = connect(sockfd,(struct sockaddr *) &server_addr, ser_len);
  if(temp < 0)
    {
      printf("Unable to connect to Socket Server\n");
      exit(0);
    }
 

  printf("How many fortune cookies do you want?\n(Max Limit 4!)\n");
  scanf("%d", &cookie_num);
  if((cookie_num > 0) && (cookie_num < 5))
    {
      sprintf(str, "%d", cookie_num);
      // sendto(sockfd, str, sizeof(str), 0, (struct sockaddr*) &server_addr, sizeof(server_addr));
      if(write(sockfd,str,1)!=1)
	printf("write error");
      //recvfrom(sockfd, message, sizeof(message), 0, NULL, NULL);
      read(sockfd,message,sizeof(message));
      printf("Here are your Fortune Cookies:\n\n%s", message);
    }
  else
    printf("Incorrect number of fortune cookies requested!\n");

  close(sockfd);
}

void get_enc_dec_server(char *argv)
{
  int sockfd, newsockfd, ser_len, temp;
  char a,option[1];
  struct sockaddr_in server_addr;

  char message[2048];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if(sockfd < 0)
    {
      printf("Socket Error\n");
      exit(0);
    }

  bzero((char *) &server_addr, sizeof(server_addr));
  bzero((char *) &message, sizeof(message));

  server_addr.sin_family = AF_INET;
  //server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  inet_aton(argv, &server_addr.sin_addr.s_addr);
  server_addr.sin_port = htons(7891);

  ser_len = sizeof(server_addr);

  temp = connect(sockfd,(struct sockaddr *) &server_addr, ser_len);
  if(temp < 0)
    {
      printf("Unable to connect to Socket Server\n");
      exit(0);
    }
  while(1)
    {
      bzero((char *) &option, sizeof(option));
      printf("\nEnter 1.Encryption\n2.Decryption\n3. Exit:");
      scanf("%s", option);
      if(write(sockfd,option,1)!=1)
	{
	  printf("write error\n");
	  close(sockfd);
	  exit(0);
	}

      switch(atol(option))
	{
	case 1:
	  printf("Encryption:Enter the text for Encryption:\n");
	  //scanf("%s", message);
	  fgetc(stdin);
	  fgets(message, 2048, stdin);
	  write(sockfd, message, sizeof(message));

	  bzero((char *) &message, sizeof(message));

	  read(sockfd, message, sizeof(message));
	  printf("Encrypted Text:%s\n",message);
	  break;
	case 2:
	  printf("Decryption:Enter the text for Decryption:\n");
	  //scanf("%s", message);
	  fgetc(stdin);
	  fgets(message, 2048, stdin);
	  write(sockfd, message, sizeof(message));

	  bzero((char *) &message, sizeof(message));

	  read(sockfd, message, sizeof(message));
	  printf("Decrypted Text:%s\n",message);
	  break;
	case 3:
	  close(sockfd);
	  return;
	default:
	  printf("Incorrect choice! Please enter again\n");
	  break;
	}
    }
}


int main(int argc, char *argv[])
{
  int ch;

  printf("---Programming Assignment 2---\n\n:");

  if(argc > 2)
    {
      printf("Enter proper arguements\n");
      exit(-1);
    }
  while(1)
    {
      printf("\nEnter your choice\n 1. Fortune Cookies\n2. Encryption/Decryption Service.\n3. Exit\t");
      scanf("%d", &ch);
      switch(ch)
{
case 1:
  get_fortune_cookie(argv[1]);
  break;
case 2:
  get_enc_dec_server(argv[1]);
  break;
case 3:
  // printf("\n!\n\n");
  return(0);
default:
  printf("Please enter valid choice \n");
  break;
}
    }
}
