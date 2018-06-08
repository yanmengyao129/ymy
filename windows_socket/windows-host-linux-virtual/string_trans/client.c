
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <errno.h>  
    #include <string.h>  
    #include <sys/types.h>  
    #include <netinet/in.h>  
    #include <sys/socket.h>  
    #include <sys/wait.h>  
    #include <arpa/inet.h>  
    #include <unistd.h> 
    #include <string.h> 
    #include <iostream>
   using namespace std;
    #define MAXDATASIZE 100  

    int main() {  
        int sockfd, recvbytes;    
        struct sockaddr_in serv_addr;
        //创建套接字
  
 while(1){ 
     //保存服务器端口和ip地址信息
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
            perror("socket error!");  
            exit(1);  
        }  

        bzero(&serv_addr, sizeof(serv_addr));  
        serv_addr.sin_family = AF_INET;  
        serv_addr.sin_port = htons(8888);  
        serv_addr.sin_addr.s_addr = inet_addr("172.24.13.15");  
      
//连接成功，传送字符
        if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr))== -1) {  
            perror("connect error!");  
            exit(1);  
        }  
        
        string data;
        cin>>data;
        if(data=="exit")break;
        const char * DATA;
        DATA=data.c_str(); 
         
        write(sockfd, DATA, sizeof(DATA));

        char recData[255];

        int ret=recv(sockfd, recData,255, 0);
       
  
        if (ret>0) {  
        printf("%s",recData);
        cout<<endl;
        }  
       
   
        close(sockfd);//关闭套接字
}  
 close(sockfd);
        return 0;  
    }  
