
#pragma comment(lib, "WS2_32")
#include <WinSock2.h>  
#include<stdio.h>       // printf  
#include<stdlib.h>      // exit  
#include<string.h>      // bzero  
  
#define SERVER_PORT 4096  
#define LENGTH_OF_LISTEN_QUEUE 20  
#define BUFFER_SIZE 1024  
#define FILE_NAME_MAX_SIZE 512  
  
int main(void)  
{  
WORD sockVersion = MAKEWORD(2,2);    
    WSADATA wsaData;    
    if(WSAStartup(sockVersion, &wsaData)!=0)    
    {    
        return 0;    
    }    
  
  	SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen==SOCKET_ERROR ){
		   printf("Init Socket Error!\n");
		   return 0;}
		   
	// 绑定socket到一个本地地址
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	
	if(bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)  {
	   printf("Bind Error!\n");
	   return 0;}
	   
	// 设置socket进入监听状态
	if (listen(sListen, 10) == SOCKET_ERROR){
	   printf("Listen Error!\n");
	   return 0;}
	printf("Listening To Client...\n");       
        SOCKET sClient;    
    sockaddr_in remoteAddr;    
    int nAddrlen = sizeof(remoteAddr);    
      
    while (true)    
    {    
        printf("等待连接...\n");    
        sClient = accept(sListen, (SOCKADDR *)&remoteAddr, &nAddrlen);
		//等待客户端接入SOCKET Command_Sock = accept(Listen_Sock,...)
		//它提取出所监听套接字的等待连接队列中第一个连接请求，创建一个新的套接字，并返回指向该套接字的文件描述符。
		//新建立的套接字不在监听状态，原来所监听的套接字也不受该系统调用的影响，新建立的套接字准备发送send()和接收数据recv()。。    
        //remoteAddr:该结构用通讯层服务器对等套接字的地址(一般为客户端地址)填写，返回地址addr的确切格式由套接字的地址类别(比如TCP或UDP)决定
		//是个指向局部数据结构sockaddr_in的指针，这就是要求接入的信息本地的套接字(地址和指针) 
		if(sClient == INVALID_SOCKET)    
        {    
            printf("accept error !");    
            continue;    
        }    
        printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));    
  
        char buffer[BUFFER_SIZE];  
        char file_name[FILE_NAME_MAX_SIZE+1];
	    int ret = recv(sClient, file_name, 255, 0);    
		if(ret<0)  
        {  
            perror("Server Recieve Data Failed:");  
            break;  
        }   

  // 对文件路径名进行一些处理
    file_name[ret]='\0';
	char *p1, *p2;
	char filename[FILE_NAME_MAX_SIZE+1];
	
  	for (p1 = file_name, p2 =filename;'\0' != *p1;++p1, ++p2)
		{
		   if ('\n' != *p1){
		    *p2 = *p1;}
		   
		   if ('\\' == *p2){
		    *(++p2) = '\\';}
		}
	*p2 = '\0';
	
        // 打开文件并读取文件数据  
        FILE *fp; 
        if (NULL == (fp = fopen(filename, "rb")))
        {  
            printf("File:%s Not Found\n", file_name);  
           break;
		}  
       
           memset(buffer, 0, BUFFER_SIZE);  
            int length = 0;  
            // 每读取一段数据，便将其发送给客户端，循环直到文件读完为止  
            while((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)  
            {  
                if(send(sClient, buffer, length, 0) < 0)  
                {  
                    printf("Send File:%s Failed./n", file_name);  
                    break;  
                }  
               memset(buffer, 0, BUFFER_SIZE);
            }  
  
            // 关闭文件  
            fclose(fp);  
            printf("File:%s Transfer Successful!\n", file_name);  
         
        // 关闭与客户端的连接  
        	closesocket(sClient);  
    }  
    // 关闭监听用的socket  
  	closesocket(sListen);  
    return 0;  
}  
