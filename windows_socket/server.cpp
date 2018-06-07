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
	    //�����׽��� ���������SOCKET����ʽSOCKET 
	//����һ���µ�Socket����Ӧ �ͻ��˵���������
	//AF_INET�ֶ��Ǳ�ʾ�����ַ�����ͣ�
	//AF_INET��ʾ��internet���н���ͨ�ţ�
	//SOCK_STREAM��ʾsocket�����ͣ�SOCK_STREAM��ʾ�����׽���
	//IPPROTO_TCP����Э������,IPPROTO_TCP����0��ʾTCP/IPЭ��   
    if(slisten == INVALID_SOCKET)    
    {    
        printf("socket error !");    
        return 0;    
    }    
    
    //��IP�Ͷ˿�    
    sockaddr_in sin;   //�����TCP/IPЭ����ϵ�������ַ��Ϣ�洢 
    /*
	struct sockaddr_in
{
    short sin_family;//Э����
    unsigned short int  sin_port;//�˿ںţ������ֽ���ֵ��windowsƽ̨���ֱ��뻷���£����ܽ�������typdefΪin_port_t��
    struct in_addr sin_addr;//intenet��ַ��Ϣ
    unsigned char sin_zero[len];//�����
}

    struct in_addr��ʹ������union�����ַ�ʽ������IP��ַ��Ϣ���� 
        {
            union
            {
                struct 
                {
                uchar s_b,s_b2,s_b3,s_b4;
                }S_un_b;//���ֽڱ��� 
                struct
                {
                ushort s_w1,s_w2;
                }S_un_w;//˫�ֽڱ��� 
                u_long S_addr;//�����ֽ��� �����ֵĸ�λ�����ڴ����ʼ��ַ�ϣ���Ϊbig_endian���涨��big_endian��Ϊ�����ֽ��� 
            }S_un;//4�ֽڱ��� 
        }    
		
		��Ϊ������洢������ǿ����Ĳ�һ����
		����ڲ���sockaddr_in��sin_port��sin_addrʱ��
		һ��Ҫ�������ֽ���Ķ˿ںź�IP��ַͨ�������ֽ����������ֽ����ת����������ת�� 
		
		 �����ֽ���ת��Ϊ�����ֽ���
    u_long htonl(u_long hostlong); //4�ֽ�
    u_short htons(u_short hostshort);  //2�ֽ�
    �����ֽ���ת��Ϊ�����ֽ���
    u_long ntohl(u_long netlong); //4�ֽ�
    u_long ntohs(u_short netshort); //2�ֽ�

*/ 
    sin.sin_family = AF_INET;// ��д��������ַ��Ϣ��AF_INET������Ҫ��ipv4��ַ��32λ�ģ���˿ںţ�16λ�ģ������    
    sin.sin_port = htons(8888);//���ؼ����˿�8888,    
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
	//INADDR_ANY��ʾ�����ϵ����������ַ ;��S_addr4�ֽ�ֱ�ӱ���IP��ַ��Ϣ 
	//������Ļ�������������������ô�������������������ݣ����socket�����Եõ�֪ͨ��
	//htonl��IP��ַת��Ϊ�����ʽ
     
    if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)    
 //�󶨼����˿�;������socket���������׽��ֺ󣬸��׽��ֲ�û���뱾����ַ�Ͷ˿ڵ���Ϣ������bind�����������Щ����  
 //bind()������һ����ַ���е��ض���ַ����socket�������ӦAF_INET��AF_INET6���ǰ�һ��ipv4��ipv6��ַ�Ͷ˿ں���ϸ���socket 
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
		//sClient:accept ��ȷִ��֮��ķ��ص��µ��׽���������
		//sendData:��Ҫ���ڴ�  ----> ���뻺�����е�����
		//strlen(sendData):�� msg �ĳ���
		//flags ����ѡ���Ӧһϵ��ϵͳ����ĺ꣬����ָ���Ժ��ַ�ʽ�����ͻ�����������,ͨ��ȡ0 
        /*����ֵ��
   �������ֵΪ������˵�����ڴ��е������ƶ����������в���ʧ�ܡ�
   �������ֵΪ >=0 ����ֵ��˵���ƶ��ɹ������ص���ֵ���ǽ��ڴ��еĶ��ٸ��ֽ�
   �ƶ����˻������С����������������˵�������ƶ��ɹ��������ܱ����ɹ����ͻ�������
   �����ݵ��Եȶ� */ 
		closesocket(sClient);    
    }    
        
    closesocket(slisten);    
    WSACleanup();    
    return 0;    
}   
