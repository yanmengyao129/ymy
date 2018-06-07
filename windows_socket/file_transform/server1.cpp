#include "TransformFile.h"
char g_szNewFileName[MAXFILEDIRLENGTH];
char g_szBuff[MAX_PACKET_SIZE + 1];
long g_lLength;
char* g_pBuff = NULL;

// ��ʼ��socket��
bool InitSocket();
// �ر�socket��
bool CloseSocket();
// ������Ϣ������Ӧ�Ĵ���
bool ProcessMsg(SOCKET sClient);
// ����Client����Ϣ
void ListenToClient();
// ���ļ�
bool OpenFile(CCSDef::TMSG_HEADER* pMsgHeader, SOCKET sClient);
// �����ļ�
bool SendFile(SOCKET sClient);
// ��ȡ�ļ����뻺����
bool ReadFile(SOCKET sClient);

int main(){
	InitSocket();
	ListenToClient();
	CloseSocket();
	return 0;
	}

void ListenToClient()
{
	// ����socket�׽���
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sListen){
		   printf("Init Socket Error!\n");
		   return;}
		   
	// ��socket��һ�����ص�ַ
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	
	if (bind(sListen, (LPSOCKADDR)&sin, sizeof(sockaddr_in)) == SOCKET_ERROR){
	   printf("Bind Error!\n");
	   return;}
	   
	// ����socket�������״̬
	if (listen(sListen, 10) == SOCKET_ERROR){
	   printf("Listen Error!\n");
	   return;}
	printf("Listening To Client...\n");
	
	// ѭ������client�˵���������
	sockaddr_in ClientAddr;
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET sClient;
	
	while (INVALID_SOCKET == (sClient = accept(sListen, (sockaddr*)&ClientAddr, &nAddrLen))){
	}
	while (true == ProcessMsg(sClient)){
	}
	// �ر�ͬ�ͻ��˵�����
	closesocket(sClient);
	closesocket(sListen);
}

bool InitSocket(){
	// ��ʼ��socket dll
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 2);
	
	if (WSAStartup(socketVersion, &wsaData) != 0){
	   printf("Init socket dll error\n");
	   return false;
	}
	return true;
}

bool CloseSocket()
{
	// �ͷ�winsock��
	WSACleanup();
	if (NULL != g_pBuff){
	   delete [] g_pBuff;
	   g_pBuff = NULL;}
	   
return true;
}

	bool ProcessMsg(SOCKET sClient){
	int nRecv = recv(sClient, g_szBuff, MAX_PACKET_SIZE + 1, 0);
	if (nRecv > 0)
	{
	   g_szBuff[nRecv] = '\0';
	}
	// ��������
	CCSDef::TMSG_HEADER* pMsgHeader = (CCSDef::TMSG_HEADER*)g_szBuff;
	switch (pMsgHeader->cMsgID)
		{
		case MSG_FILENAME:    // �ļ���
		    OpenFile(pMsgHeader, sClient);
		    break;
		case MSG_CLIENT_READY:   // �ͻ���׼������,��ʼ�����ļ�
		    SendFile(sClient);
		    break;
		case MSG_SENDFILESUCCESS: // �����ļ��ɹ�
		    printf("Send File Success!\n");
		    return false;
		    break;
		case MSG_FILEALREADYEXIT_ERROR: // Ҫ������ļ��Ѿ�������
		    printf("The file reay to send already exit!\n");
		    return false;
	        break;
	    }
return true;
}

bool ReadFile(SOCKET sClient){
	if (NULL != g_pBuff){
	   return true;
	}
	
	// ���ļ�
	FILE *pFile;
	if (NULL == (pFile = fopen(g_szNewFileName, "rb")))   // ���ļ�ʧ��
		{
		   printf("Cannot find the file, request the client input file name again\n");
		   CCSDef::TMSG_ERROR_MSG tMsgErrorMsg(MSG_OPENFILE_ERROR);
		   send(sClient, (char*)(&tMsgErrorMsg), sizeof(CCSDef::TMSG_ERROR_MSG), 0);
		   return false;
		}
	// ���ļ��ĳ��ȴ��ص�clientȥ
	fseek(pFile, 0, SEEK_END);
	g_lLength = ftell(pFile);
	printf("File Length = %d\n", g_lLength);
	CCSDef::TMSG_FILELENGTH tMsgFileLength(g_lLength);
	send(sClient, (char*)(&tMsgFileLength), sizeof(CCSDef::TMSG_FILELENGTH), 0);
	
	// �����ļ�ȫ·����,���ļ����ֽ����
	char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFname[_MAX_FNAME], szExt[_MAX_EXT];
	_splitpath(g_szNewFileName, szDrive, szDir, szFname, szExt);
	strcat(szFname,szExt);
	CCSDef::TMSG_FILENAME tMsgFileName;
	strcpy(tMsgFileName.szFileName, szFname);
	printf("Send File Name: %s\n", tMsgFileName.szFileName);
	send(sClient, (char*)(&tMsgFileName), sizeof(CCSDef::TMSG_FILENAME), 0);
	
	// ���仺������ȡ�ļ�����
	g_pBuff = new char[g_lLength + 1];
	if (NULL == g_pBuff)
	{
	   return false;
	}
	fseek(pFile, 0, SEEK_SET);
	fread(g_pBuff, sizeof(char), g_lLength, pFile);
	g_pBuff[g_lLength] = '\0';
	fclose(pFile);
return true;
}
// ���ļ�
bool OpenFile(CCSDef::TMSG_HEADER* pMsgHeader, SOCKET sClient){
	CCSDef::TMSG_FILENAME* pRequestFilenameMsg = (CCSDef::TMSG_FILENAME*)pMsgHeader;
	// ���ļ�·��������һЩ����
	char *p1, *p2;
	for (p1 = pRequestFilenameMsg->szFileName, p2 = g_szNewFileName;'\0' != *p1;++p1, ++p2)
		{
		   if ('\n' != *p1){
		    *p2 = *p1;}
		   
		   if ('\\' == *p2){
		    *(++p2) = '\\';}
		}
	*p2 = '\0';
	ReadFile(sClient);
return true;
}
// �����ļ�
bool SendFile(SOCKET sClient){
	if (NULL == g_pBuff){
	   ReadFile(sClient);}
	   
	int nPacketBufferSize = MAX_PACKET_SIZE - 2 * sizeof(int); // ÿ�����ݰ�����ļ���buffer��С
	// ����ļ��ĳ��ȴ���ÿ�����ݰ����ܴ��͵�buffer������ô�ͷֿ鴫��
	
	for (int i = 0; i < g_lLength; i += nPacketBufferSize)
	{  
	   CCSDef::TMSG_FILE tMsgFile;
	   tMsgFile.tFile.nStart = i;
	   
	   if (i + nPacketBufferSize + 1> g_lLength){
	    tMsgFile.tFile.nSize = g_lLength - i;}
	   else{
	    tMsgFile.tFile.nSize = nPacketBufferSize;}
	   
	   printf("start = %d, size = %d\n", tMsgFile.tFile.nStart, tMsgFile.tFile.nSize);
	   memcpy(tMsgFile.tFile.szBuff, g_pBuff + tMsgFile.tFile.nStart, tMsgFile.tFile.nSize);
	   send(sClient, (char*)(&tMsgFile), sizeof(CCSDef::TMSG_FILE), 0);
	   Sleep(0.5);
	}
	delete [] g_pBuff;
	g_pBuff = NULL;
return true;
}
