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
	    //创建套接字 ，构造监听SOCKET，流式SOCKET 
	//创建一个新的Socket来响应 客户端的连接请求；
	//AF_INET字段是表示网络地址的类型，
	//AF_INET表示是internet域中进行通信；
	//SOCK_STREAM表示socket的类型，SOCK_STREAM表示是流套接字
	//IPPROTO_TCP表明协议类型,IPPROTO_TCP或者0表示TCP/IP协议   
    if(slisten == INVALID_SOCKET)    
    {    
        printf("socket error !");    
        return 0;    
    }    
    
    //绑定IP和端口    
    sockaddr_in sin;   //针对于TCP/IP协议体系的网络地址信息存储 
    /*
	struct sockaddr_in
{
    short sin_family;//协议族
    unsigned short int  sin_port;//端口号，网络字节序赋值（windows平台部分编译环境下，可能将此类型typdef为in_port_t）
    struct in_addr sin_addr;//intenet地址信息
    unsigned char sin_zero[len];//充填部分
}

    struct in_addr（使用联合union，三种方式来保存IP地址信息；） 
        {
            union
            {
                struct 
                {
                uchar s_b,s_b2,s_b3,s_b4;
                }S_un_b;//单字节保存 
                struct
                {
                ushort s_w1,s_w2;
                }S_un_w;//双字节保存 
                u_long S_addr;//网络字节序： 将数字的高位存在内存的起始地址上，称为big_endian，规定将big_endian称为网络字节序 
            }S_un;//4字节保存 
        }    
		
		因为计算机存储序和我们看到的不一样，
		因此在操作sockaddr_in里sin_port和sin_addr时，
		一定要将网络字节序的端口号和IP地址通过网络字节序与主机字节序的转换函数进行转换 
		
		 主机字节序转换为网络字节序：
    u_long htonl(u_long hostlong); //4字节
    u_short htons(u_short hostshort);  //2字节
    网络字节序转换为主机字节序：
    u_long ntohl(u_long netlong); //4字节
    u_long ntohs(u_short netshort); //2字节

*/ 
    sin.sin_family = AF_INET;// 填写服务器地址信息，AF_INET决定了要用ipv4地址（32位的）与端口号（16位的）的组合    
    sin.sin_port = htons(8888);//本地监听端口8888,    
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
	//INADDR_ANY表示机器上的所用网络地址 ;用S_addr4字节直接保存IP地址信息 
	//比如你的机器上有两张网卡，那么到达这两张网卡的数据，你的socket都可以得到通知；
	//htonl将IP地址转换为网络格式
     
    if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)    
 //绑定监听端口;当调用socket函数创建套接字后，该套接字并没有与本机地址和端口等信息相连，bind函数将完成这些工作  
 //bind()函数把一个地址族中的特定地址赋给socket。例如对应AF_INET、AF_INET6就是把一个ipv4或ipv6地址和端口号组合赋给socket 
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
		//sClient:accept 正确执行之后的返回的新的套接字描述符
		//sendData:是要从内存  ----> 放入缓冲区中的数据
		//strlen(sendData):是 msg 的长度
		//flags 控制选项，对应一系列系统定义的宏，用来指定以何种方式来发送缓冲区的数据,通常取0 
        /*返回值：
   如果返回值为负数，说明将内存中的数据移动到缓冲区中操作失败。
   如果返回值为 >=0 的数值，说明移动成功，返回的数值便是将内存中的多少个字节
   移动到了缓冲区中。而且这种情况仅仅说明数据移动成功，并不能表明成功发送缓冲区中
   的数据到对等端 */ 
		closesocket(sClient);    
    }    
        
    closesocket(slisten);    
    WSACleanup();    
    return 0;    
}   
