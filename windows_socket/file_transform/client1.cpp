#include "TransformFile.h"
long g_lLength = 0;
char* g_pBuff = NULL;
char g_szFileName[MAXFILEDIRLENGTH];
char g_szBuff[MAX_PACKET_SIZE + 1];
SOCKET g_sClient;
// 初始化socket库
bool InitSocket();

// 关闭socket库
bool CloseSocket();

// 把用户输入的文件路径传送到server端
bool SendFileNameToServer();

// 与server端连接
bool ConectToServer();

// 打开文件失败
bool OpenFileError(CCSDef::TMSG_HEADER *pMsgHeader);

// 分配空间以便写入文件
bool AllocateMemoryForFile(CCSDef::TMSG_HEADER *pMsgHeader);

// 写入文件
bool WriteToFile(CCSDef::TMSG_HEADER *pMsgHeader);

// 处理server端传送过来的消息
bool ProcessMsg();

int main()
{
	InitSocket();
	ConectToServer();
	CloseSocket();
	return 0;
}

// 初始化socket库
bool InitSocket(){
	// 初始化socket dll
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 2);
	
	if (WSAStartup(socketVersion, &wsaData) != 0){
	   printf("Init socket dll error\n");
	   exit(-1);}
   
return true;
}
// 关闭socket库
bool CloseSocket(){
	// 关闭套接字
	closesocket(g_sClient);
	// 释放winsock库
	WSACleanup();
return true;
}

// 与server端连接进行文件的传输
bool ConectToServer(){
	// 初始化socket套接字
	if (SOCKET_ERROR == (g_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))){
	   printf("Init Socket Error!\n");
	   exit(-1);
	}
	
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	servAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	
	if (INVALID_SOCKET == (connect(g_sClient, (sockaddr*)&servAddr, sizeof(sockaddr_in)))){
	   printf("Connect to Server Error!\n");
	   exit(-1);
	}
	
	// 输入文件路径传输到server端
	SendFileNameToServer();
	// 接收server端传过来的信息,直到保存文件成功为止
	while (true == ProcessMsg())
	{
	}
return true;
}
// 把用户输入的文件路径传送到server端
bool SendFileNameToServer(){
	char szFileName[MAXFILEDIRLENGTH];
	printf("Input the File Directory: ");
	
	fgets(szFileName, MAXFILEDIRLENGTH, stdin);
	// 把文件路径发到server端
	CCSDef::TMSG_FILENAME tMsgRequestFileName;
	strcpy(tMsgRequestFileName.szFileName, szFileName);
	
	if (SOCKET_ERROR == send(g_sClient, (char*)(&tMsgRequestFileName), sizeof(CCSDef::TMSG_FILENAME), 0)){
	   printf("Send File Name Error!\n");
	   exit(-1);
	}
return true;
}
// 处理server端传送过来的消息
bool ProcessMsg(){
	CCSDef::TMSG_HEADER *pMsgHeader;
	int nRecv = recv(g_sClient, g_szBuff, MAX_PACKET_SIZE + 1, 0);
	
	pMsgHeader = (CCSDef::TMSG_HEADER*)g_szBuff;
	switch (pMsgHeader->cMsgID){
	case MSG_OPENFILE_ERROR:   // 打开文件错误
	    OpenFileError(pMsgHeader);
	   break;
	case MSG_FILELENGTH:    // 文件的长度
	    if (0 == g_lLength)
	    {
	     g_lLength = ((CCSDef::TMSG_FILELENGTH*)pMsgHeader)->lLength;
	     printf("File Length: %d\n", g_lLength);
	    }
	   break;
	case MSG_FILENAME:     // 文件名
	    return AllocateMemoryForFile(pMsgHeader);
	   break;
	case MSG_FILE:      // 传送文件,写入文件成功之后退出这个函数
	    if (WriteToFile(pMsgHeader))
	    {
	     return false;
	    } 
	   break;
	}
return true;
}
// 打开文件失败
bool OpenFileError(CCSDef::TMSG_HEADER *pMsgHeader){
	if (NULL != g_pBuff)
	   return true;
	   
	assert(NULL != pMsgHeader);
	printf("Cannot find file!Please input again!\n");
	// 重新输入文件名称
	SendFileNameToServer();
return true;
}

// 查找是否已经存在了要保存的文件,同时分配缓冲区保存文件
bool AllocateMemoryForFile(CCSDef::TMSG_HEADER *pMsgHeader){
	assert(NULL != pMsgHeader);
	
	if (NULL != g_pBuff){
	   return true;}
	
	CCSDef::TMSG_FILENAME* pRequestFilenameMsg = (CCSDef::TMSG_FILENAME*)pMsgHeader;
	printf("File Name: %s\n", pRequestFilenameMsg->szFileName);

	// 把文件的路径设置为D盘根目录下
	strcpy(g_szFileName, "D:\\");
	strcat(g_szFileName, pRequestFilenameMsg->szFileName);

	// 查找相同文件名的文件是否已经存在,如果存在报错退出
	FILE* pFile;
	if (NULL != (pFile = fopen(g_szFileName, "r"))){
	   // 文件已经存在,要求重新输入一个文件
	   printf("The file already exist!\n");
	   CCSDef::TMSG_ERROR_MSG tMsgErrorMsg(MSG_FILEALREADYEXIT_ERROR);
	   send(g_sClient, (char*)(&tMsgErrorMsg), sizeof(CCSDef::TMSG_ERROR_MSG), 0);
	   fclose(pFile);
	   return false;}
	   
	// 分配缓冲区开始接收文件,如果分配成功就给server端发送开始传送文件的要求
	g_pBuff = new char[g_lLength + 1];
	if (NULL != g_pBuff){
	   memset(g_pBuff, '\0', g_lLength + 1);
	   printf("Now ready to get the file %s!\n", pRequestFilenameMsg->szFileName);
	   CCSDef::TMSG_CLIENT_READY tMsgClientReady;
	   
	   if (SOCKET_ERROR == send(g_sClient, (char*)(&tMsgClientReady), sizeof(CCSDef::TMSG_CLIENT_READY), 0)){
	    printf("Send Error!\n");
	    exit(-1);
	   }
	}
	else{
	   printf("Alloc memory for file error!\n");
	   exit(-1);}
return true;
}
// 写入文件
bool WriteToFile(CCSDef::TMSG_HEADER *pMsgHeader){
	assert(NULL != pMsgHeader);
	CCSDef::TMSG_FILE* pMsgFile = (CCSDef::TMSG_FILE*)pMsgHeader;
	int nStart = pMsgFile->tFile.nStart;
	int nSize = pMsgFile->tFile.nSize;
	memcpy(g_pBuff + nStart, pMsgFile->tFile.szBuff, nSize);
	if (0 == nStart)
	{
	   printf("Saving file into buffer...\n");
	}
	memcpy(g_pBuff + nStart, pMsgFile->tFile.szBuff, nSize);
	//printf("start = %d, size = %d\n", nStart, nSize);
	
	// 如果已经保存到缓冲区完毕就写入文件
	if (nStart + nSize >= g_lLength){
	   printf("Writing to disk....\n");
	  
	   // 写入文件
	   FILE* pFile;
	   pFile = fopen(g_szFileName, "w+b");
	   fwrite(g_pBuff, sizeof(char), g_lLength, pFile);
	   delete [] g_pBuff;
	   g_pBuff = NULL;
	   fclose(pFile);
	   
	   // 保存文件成功传送消息给server退出server
	   CCSDef::TMSG_SENDFILESUCCESS tMsgSendFileSuccess;
	   while (SOCKET_ERROR == send(g_sClient, (char*)(&tMsgSendFileSuccess), sizeof(CCSDef::TMSG_SENDFILESUCCESS), 0)){
	   }
	   
	   printf("Save the file %s success!\n", g_szFileName);
	   return true;
	}
	
	else{
	   return false;
	}
}

