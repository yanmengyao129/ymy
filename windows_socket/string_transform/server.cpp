#include <stdio.h>    
#include <winsock2.h>    
#include<iostream>  
#include<cstring>  
using namespace std;  
#pragma comment(lib,"ws2_32.lib")    
    
int main(int argc, char* argv[])    
{    
    //初始化WSA    
    WORD sockVersion = MAKEWORD(2,2);    
    WSADATA wsaData;    
    if(WSAStartup(sockVersion, &wsaData)!=0)    
    {    
        return 0;    
    }    
    

    SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
   
    if(slisten == INVALID_SOCKET)    
    {    
        printf("socket error !");    
        return 0;    
    }    
    
    //绑定IP和端口    
    sockaddr_in sin;   //针对于TCP/IP协议体系的网络地址信息存储 

    sin.sin_family = AF_INET;// 填写服务器地址信息，AF_INET决定了要用ipv4地址（32位的）与端口号（16位的）的组合    
    sin.sin_port = htons(8888);//本地监听端口8888,    
    sin.sin_addr.S_un.S_addr = INADDR_ANY;

     
    if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)    

    {    
        printf("bind error !");    
    }    
    
      
    if(listen(slisten, 5) == SOCKET_ERROR) 
	// 开始监听，指定最大同时连接数为2   
    {    
        printf("listen error !");    
        return 0;    
    }    
    
    //循环接收数据    
    SOCKET sClient;    
    sockaddr_in remoteAddr;    
    int nAddrlen = sizeof(remoteAddr);    
    char revData[255];     
    while (true)    
    {    
        printf("等待连接...\n");    
        sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if(sClient == INVALID_SOCKET)    
        {    
            printf("accept error !");    
            continue;    
        }    
        printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));    
            
        //接收数据    
        int ret = recv(sClient, revData, 255, 0);           
        if(ret > 0)    
        {    
            cout<<"client:";
            revData[ret] = 0x00;    
            printf(revData); 
			cout<<endl;   
        }    
    
        //发送数据    
		string data;  
        cin>>data;  
        if(data=="exit")break;
        const char * sendData;  
        sendData = data.c_str();   //string转const char*    
        send(sClient, sendData, strlen(sendData), 0);
		closesocket(sClient);    
    }    
        
    closesocket(slisten);    
    WSACleanup();    
    return 0;    
}   
