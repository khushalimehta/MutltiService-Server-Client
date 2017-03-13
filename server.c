#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

struct s_socket
{
  int flag;
  char data[200];
};

struct s_socket cookielist[50];
//Fortune cookie server function
void *fortune_cserver()
{
  int sockfd, newsockfd, sock_len;
  int fid, i, r_pos, flag1,client_len;
  struct sockaddr_in server_addr, client_addr;
  char cookie_num[1], index[3];

  char message[2048];
  printf("hello server\n");

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  // sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0)
    {
      printf("Socket Error \n");
      exit(0);
    }
  bzero((char *) &server_addr, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  //server_addr.sin_port = htons(7891);
 server_addr.sin_port = htons(7070);

  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  sock_len = sizeof(server_addr);
  if(bind(sockfd, (struct sockaddr *) &server_addr, sock_len) < 0)
    {
      printf("Bind Error\n");
      close(sockfd);
      exit(0);
    }
 if((listen(sockfd, 5)) < 0)
    {
      printf("Listen Error\n");
      close(sockfd);
      exit(0);
    }

  for(;;)
    {
      sock_len = sizeof(client_addr);
      bzero((char *) &message, sizeof(message));
      client_len=sizeof(client_addr);
      newsockfd = accept(sockfd,(struct sockaddr *) &client_addr, &sock_len);
      if(newsockfd < 0)
	{
	  printf("Accept Error\n");
	  close(sockfd);
	  exit(0);
	}
     
      //recvfrom(newsockfd,cookie_num,1, 0,(struct sockaddr*)&client_addr,&client_len);
      read(newsockfd,message, sizeof(message));
      for(i = 1; i <= atol(cookie_num); i++)
	{
	  flag1 = 0;
	  while(flag1 == 0)
	    {
	      srand(time(NULL));
	      r_pos = rand() % 50;
	      if(cookielist[r_pos].flag == 0)
		{
		  sprintf(index, "%d. ", i);
		  strcat(message, index);
		  strcat(message, cookielist[r_pos].data);
		  cookielist[r_pos].flag = 1;
		  flag1 = 1;
		}
	      sleep(1);
	    }
	}

      //sendto(newsockfd, message,sizeof(message), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
      write(newsockfd,message, sizeof(message));
      close(newsockfd);
    }
}

void *enc_dec_server()
{
  int sockfd, newsockfd, sock_len, temp, fid;
  struct sockaddr_in server_addr, client_addr;
  char opt[1],readchoice[1];
  int i, l;
  char message[2048];
  char p[2098], q[2098];
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0)
    {
      printf("Socket Error\n");
      exit(0);
    }
  bzero((char *) &server_addr, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(7891);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);


  sock_len = sizeof(server_addr);
  temp = bind(sockfd, (struct sockaddr *) &server_addr, sock_len);
  if(temp < 0)
    {
      printf("Bind Error\n");
      close(sockfd);
      exit(0);
    }


  if((listen(sockfd, 5)) < 0)
    {
      printf("Listen Error\n");
      close(sockfd);
      exit(0);
    }

  for(;;)
    {
      sock_len = sizeof(client_addr);
      newsockfd = accept(sockfd,(struct sockaddr *) &client_addr, &sock_len);
      if(newsockfd < 0)
	{
	  printf("Accept Error\n");
	  close(sockfd);
	  exit(0);
	}
      fid = fork();
      if(fid == 0)
	{
	  close(sockfd);
	  for(;;){
	    if(read(newsockfd, readchoice, 1)<0)
	      {
		printf("Read Error\n");
		close(newsockfd);
		exit(0);
	      }
	    bzero((char *) &message, sizeof(message));
	    if(read(newsockfd, message, sizeof(message))<0)
	      {
		printf("Read Error\n");
		close(newsockfd);
		exit(0);
	      }
	    //sleep(1);
	    switch(atol(readchoice))
	      {
	      case 1:
		l = strlen(message);

		for(i = 0; i < l; i++)
		  message[i] += 3;
		message[l] = '\0';
		printf("message:%s\n",message);
		write(newsockfd,message, sizeof(message));
		break;
	      case 2:
		l = strlen(message);
		for(i = 0; i < l; i++)
		  message[i] -= 3;
		message[l] = '\0';
		printf("message:%s\n",message);
		write(newsockfd, message, sizeof(message));
		break;
	      case 3:
		close(newsockfd);
		exit(0);
	      }
	  }
	}
      else
	{
	  close(newsockfd);
	}

    }
}
int main()
{
  pthread_t service1, service2;

  char buff[200];
  FILE *file;
  size_t nread;
  int i;

  file = fopen("fortune.txt", "r");
  if(file)
    {
      i = 0;
      while(fgets(buff,200, file) != NULL)
	{
	  strcpy(cookielist[i].data,buff);
	  cookielist[i].flag = 0;
	  i++;
	}
    }
  else
    {
      printf("Error in Opening File.\n");

      fclose(file);
      exit(-1);
    }

  fclose(file);

  printf("\nServer is Running\n");
  pthread_create(&service1, NULL, fortune_cserver, NULL);
  pthread_create(&service2, NULL, enc_dec_server, NULL);

  pthread_join(service1, NULL);
  pthread_join(service2, NULL);

}



