#include <stdio.h>    
#include <winsock2.h>    
#include<iostream>  
#include<cstring>  
using namespace std;  
#pragma comment(lib,"ws2_32.lib")    
    
int main(int argc, char* argv[])    
{    
    //��ʼ��WSA    
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
    
    //��IP�Ͷ˿�    
    sockaddr_in sin;   //�����TCP/IPЭ����ϵ�������ַ��Ϣ�洢 

    sin.sin_family = AF_INET;// ��д��������ַ��Ϣ��AF_INET������Ҫ��ipv4��ַ��32λ�ģ���˿ںţ�16λ�ģ������    
    sin.sin_port = htons(8888);//���ؼ����˿�8888,    
    sin.sin_addr.S_un.S_addr = INADDR_ANY;

     
    if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)    

    {    
        printf("bind error !");    
    }    
    
      
    if(listen(slisten, 5) == SOCKET_ERROR) 
	// ��ʼ������ָ�����ͬʱ������Ϊ2   
    {    
        printf("listen error !");    
        return 0;    
    }    
    
    //ѭ����������    
    SOCKET sClient;    
    sockaddr_in remoteAddr;    
    int nAddrlen = sizeof(remoteAddr);    
    char revData[255];     
    while (true)    
    {    
        printf("�ȴ�����...\n");    
        sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if(sClient == INVALID_SOCKET)    
        {    
            printf("accept error !");    
            continue;    
        }    
        printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));    
            
        //��������    
        int ret = recv(sClient, revData, 255, 0);           
        if(ret > 0)    
        {    
            cout<<"client:";
            revData[ret] = 0x00;    
            printf(revData); 
			cout<<endl;   
        }    
    
        //��������    
		string data;  
        cin>>data;  
        if(data=="exit")break;
        const char * sendData;  
        sendData = data.c_str();   //stringתconst char*    
        send(sClient, sendData, strlen(sendData), 0);
		closesocket(sClient);    
    }    
        
    closesocket(slisten);    
    WSACleanup();    
    return 0;    
}   
