
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
		   
	// ��socket��һ�����ص�ַ
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	
	if(bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)  {
	   printf("Bind Error!\n");
	   return 0;}
	   
	// ����socket�������״̬
	if (listen(sListen, 10) == SOCKET_ERROR){
	   printf("Listen Error!\n");
	   return 0;}
	printf("Listening To Client...\n");       
        SOCKET sClient;    
    sockaddr_in remoteAddr;    
    int nAddrlen = sizeof(remoteAddr);    
      
    while (true)    
    {    
        printf("�ȴ�����...\n");    
        sClient = accept(sListen, (SOCKADDR *)&remoteAddr, &nAddrlen);
		//�ȴ��ͻ��˽���SOCKET Command_Sock = accept(Listen_Sock,...)
		//����ȡ���������׽��ֵĵȴ����Ӷ����е�һ���������󣬴���һ���µ��׽��֣�������ָ����׽��ֵ��ļ���������
		//�½������׽��ֲ��ڼ���״̬��ԭ�����������׽���Ҳ���ܸ�ϵͳ���õ�Ӱ�죬�½������׽���׼������send()�ͽ�������recv()����    
        //remoteAddr:�ýṹ��ͨѶ��������Ե��׽��ֵĵ�ַ(һ��Ϊ�ͻ��˵�ַ)��д�����ص�ַaddr��ȷ�и�ʽ���׽��ֵĵ�ַ���(����TCP��UDP)����
		//�Ǹ�ָ��ֲ����ݽṹsockaddr_in��ָ�룬�����Ҫ��������Ϣ���ص��׽���(��ַ��ָ��) 
		if(sClient == INVALID_SOCKET)    
        {    
            printf("accept error !");    
            continue;    
        }    
        printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));    
  
        char buffer[BUFFER_SIZE];  
        char file_name[FILE_NAME_MAX_SIZE+1];
	    int ret = recv(sClient, file_name, 255, 0);    
		if(ret<0)  
        {  
            perror("Server Recieve Data Failed:");  
            break;  
        }   

  // ���ļ�·��������һЩ����
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
	
        // ���ļ�����ȡ�ļ�����  
        FILE *fp; 
        if (NULL == (fp = fopen(filename, "rb")))
        {  
            printf("File:%s Not Found\n", file_name);  
           break;
		}  
       
           memset(buffer, 0, BUFFER_SIZE);  
            int length = 0;  
            // ÿ��ȡһ�����ݣ��㽫�䷢�͸��ͻ��ˣ�ѭ��ֱ���ļ�����Ϊֹ  
            while((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)  
            {  
                if(send(sClient, buffer, length, 0) < 0)  
                {  
                    printf("Send File:%s Failed./n", file_name);  
                    break;  
                }  
               memset(buffer, 0, BUFFER_SIZE);
            }  
  
            // �ر��ļ�  
            fclose(fp);  
            printf("File:%s Transfer Successful!\n", file_name);  
         
        // �ر���ͻ��˵�����  
        	closesocket(sClient);  
    }  
    // �رռ����õ�socket  
  	closesocket(sListen);  
    return 0;  
}  
