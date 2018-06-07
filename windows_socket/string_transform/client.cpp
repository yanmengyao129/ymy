//流程：加载ws2_32.dll    ==》 创建套字节 ==》 绑定端口  ==》监听  ==》 接收/发送  ==》 关闭套字节   ==》 释放ws2_32.dll
#include<WINSOCK2.H>  
#include<STDIO.H>  
#include<iostream>  
#include<cstring>  
using namespace std;  
#pragma comment(lib, "ws2_32.lib")  
  
int main()  
{  
    WORD sockVersion = MAKEWORD(2, 2); //指明希望使用的winsock版本号 
    WSADATA data;  //WSA结构体 

    if(WSAStartup(sockVersion, &data)!=0) //WSAStartup()函数进行初始化， 
    {  
        return 0;  //初始化socket资源，return代表失败 
    }  
    
    while(true){ 
	//创建套接字 ， 构造通讯SOCKET，流式SOCKET
        SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
        if(sclient == INVALID_SOCKET)  
        {  
            printf("invalid socket!");  
            return 0;  
        }  
        
        //配置要连接的地址和端口   
        sockaddr_in serAddr;  
        serAddr.sin_family = AF_INET;  
        serAddr.sin_port = htons(8888);  
        serAddr.sin_addr.S_un.S_addr = inet_addr("172.24.13.15");  
        if(connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)  
        //客户端：请求与服务端连接int ret = connect(Client_Sock, ...) 
		{  //连接失败   
            printf("connect error !");  
            closesocket(sclient);  
            return 0;  
        }  
          
        string data;  
        cin>>data;  
        if(data=="exit")break;
        const char * sendData;  
        sendData = data.c_str();   //string转const char*   
        send(sclient, sendData, strlen(sendData), 0);  
        //send()用来将数据由指定的socket传给对方主机 ） 
          
        char recData[255];  
        int ret = recv(sclient, recData, 255, 0);  
        if(ret>0){  
            cout<<"server:";
            recData[ret] = 0x00;  
            printf(recData);  
        }   
        closesocket(sclient);  
    }  
      
      
    WSACleanup();  
    //关闭套接字，关闭加载的套接字库（closesocket()/WSACleanup()）； 
    return 0;  
      
}  
