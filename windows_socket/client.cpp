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
    
/*typedef struct WSAData {
WORD wVersion; //ws2_32.dll ��������ʹ�õİ汾��
WORD wHighVersion; //ws2_32.dll ֧�ֵ���߰汾�ţ�һ���� null ��β���ַ���������˵�� ws2_32.dll ��ʵ���Լ�������Ϣ
char szDescription[WSADESCRIPTION_LEN+1];һ���� null ��β���ַ���������˵�� ws2_32.dll ��״̬�Լ�������Ϣ
char szSystemStatus[WSASYS_STATUS_LEN+1];
unsigned short iMaxSockets; //2.0�Ժ���ʹ��
unsigned short iMaxUdpDg; //2.0�Ժ���ʹ��
char FAR *lpVendorInfo; //2.0�Ժ���ʹ��
} WSADATA, *LPWSADATA;

WinSock ��̵ĵ�һ�����Ǽ��� ws2_32.dll��Ȼ����� WSAStartup() �������г�ʼ������ָ��Ҫʹ�õİ汾��
*/ 
    
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
        //serAddr:���� conenct �����Ŀͻ��˷�������ķ���������ַ���� 
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
        //char * sendData = "��ã�TCP����ˣ����ǿͻ���\n";  
        send(sclient, sendData, strlen(sendData), 0);  
        //send()������������ָ����socket�����Է�����  
        //int send(int s, const void * msg, int len, unsigned int flags)  
        //sΪ�ѽ��������ӵ�socket��msgָ���������ݣ�len��Ϊ���ݳ��ȣ�����flagsһ����0  
        //�ɹ��򷵻�ʵ�ʴ��ͳ�ȥ���ַ�����ʧ�ܷ���-1������ԭ�����error   
          
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
