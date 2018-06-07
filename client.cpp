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
    
/*typedef struct WSAData {
WORD wVersion; //ws2_32.dll 建议我们使用的版本号
WORD wHighVersion; //ws2_32.dll 支持的最高版本号，一个以 null 结尾的字符串，用来说明 ws2_32.dll 的实现以及厂商信息
char szDescription[WSADESCRIPTION_LEN+1];一个以 null 结尾的字符串，用来说明 ws2_32.dll 的状态以及配置信息
char szSystemStatus[WSASYS_STATUS_LEN+1];
unsigned short iMaxSockets; //2.0以后不再使用
unsigned short iMaxUdpDg; //2.0以后不再使用
char FAR *lpVendorInfo; //2.0以后不再使用
} WSADATA, *LPWSADATA;

WinSock 编程的第一步就是加载 ws2_32.dll，然后调用 WSAStartup() 函数进行初始化，并指明要使用的版本号
*/ 
    
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
        //serAddr:运行 conenct 函数的客户端发送请求的服务端网络地址变量 
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
        //char * sendData = "你好，TCP服务端，我是客户端\n";  
        send(sclient, sendData, strlen(sendData), 0);  
        //send()用来将数据由指定的socket传给对方主机  
        //int send(int s, const void * msg, int len, unsigned int flags)  
        //s为已建立好连接的socket，msg指向数据内容，len则为数据长度，参数flags一般设0  
        //成功则返回实际传送出去的字符数，失败返回-1，错误原因存于error   
          
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
