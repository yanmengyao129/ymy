//���̣�����ws2_32.dll    ==�� �������ֽ� ==�� �󶨶˿�  ==������  ==�� ����/����  ==�� �ر����ֽ�   ==�� �ͷ�ws2_32.dll
#include<WINSOCK2.H>  
#include<STDIO.H>  
#include<iostream>  
#include<cstring>  
using namespace std;  
#pragma comment(lib, "ws2_32.lib")  
  
int main()  
{  
    WORD sockVersion = MAKEWORD(2, 2); //ָ��ϣ��ʹ�õ�winsock�汾�� 
    WSADATA data;  //WSA�ṹ�� 

    if(WSAStartup(sockVersion, &data)!=0) //WSAStartup()�������г�ʼ���� 
    {  
        return 0;  //��ʼ��socket��Դ��return����ʧ�� 
    }  
    
    while(true){ 
	//�����׽��� �� ����ͨѶSOCKET����ʽSOCKET
        SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
        if(sclient == INVALID_SOCKET)  
        {  
            printf("invalid socket!");  
            return 0;  
        }  
        
        //����Ҫ���ӵĵ�ַ�Ͷ˿�   
        sockaddr_in serAddr;  
        serAddr.sin_family = AF_INET;  
        serAddr.sin_port = htons(8888);  
        serAddr.sin_addr.S_un.S_addr = inet_addr("172.24.13.15");  
        if(connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)  
        //�ͻ��ˣ���������������int ret = connect(Client_Sock, ...) 
		{  //����ʧ��   
            printf("connect error !");  
            closesocket(sclient);  
            return 0;  
        }  
          
        string data;  
        cin>>data;  
        if(data=="exit")break;
        const char * sendData;  
        sendData = data.c_str();   //stringתconst char*   
        send(sclient, sendData, strlen(sendData), 0);  
        //send()������������ָ����socket�����Է����� �� 
          
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
    //�ر��׽��֣��رռ��ص��׽��ֿ⣨closesocket()/WSACleanup()���� 
    return 0;  
      
}  
