// Serverweiqi.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "afagou.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;
HANDLE bufferMutex;		// �����ܻ���ɹ�����ͨ�ŵ��ź������
SOCKET sockConn;		// �ͻ��˵��׽���
vector <SOCKET> clientSocketGroup;
string talk;
string tal;
weiqi weiqi001;
char flag = '*';
int main()
{
	// ����socket��̬���ӿ�(dll)
	WORD wVersionRequested;
	WSADATA wsaData;	// ��ṹ�����ڽ���Wjndows Socket�Ľṹ��Ϣ��
	wVersionRequested = MAKEWORD(2, 2);	// ����2.2�汾��WinSock��
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return -1;			// ����ֵΪ���ʱ���Ǳ�ʾ�ɹ�����WSAStartup
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) { // ����Ƿ�2.2�汾��socket��
		WSACleanup();
		return -1;
	}

	// ����socket������������ʽ�׽��֣������׽��ֺ�sockSrv
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	// �׽���sockSrv�뱾�ص�ַ����
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // ��INADDR_ANYת��Ϊ�����ֽ��򣬵��� htonl(long��)��htons(����)
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	if (SOCKET_ERROR == bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR))){ // �ڶ�����Ҫǿ������ת��
		return -1;
	}

	// ���׽�������Ϊ����ģʽ���������󣩣� listen()֪ͨTCP������׼���ý�������
	listen(sockSrv, 20);

	cout << "�������ѳɹ����������������뷢����Ϣ���ͻ��ˣ���ֱ���������ݺ󰴻س�.\n";
	// accept()���������ӣ��ȴ��ͻ�������

	bufferMutex = CreateSemaphore(NULL, 1, 1, NULL);

	DWORD WINAPI ReceiveMessageThread(LPVOID IpParameter);

	DWORD WINAPI SendMessageThread(LPVOID IpParameter);
	
	cout << talk << endl;
	while (true){	// ���ϵȴ��ͻ�������ĵ���
		sockConn = accept(sockSrv, NULL, NULL);
		if (SOCKET_ERROR != sockConn){
			clientSocketGroup.push_back(sockConn);
		}
		HANDLE receiveThread = CreateThread(NULL, 0, ReceiveMessageThread, (LPVOID)sockConn, 0, NULL);
		//HANDLE sendThread = CreateThread(NULL, 0, SendMessageThread, NULL, 0, NULL);
		WaitForSingleObject(bufferMutex, INFINITE);		// P����Դδ��ռ�ã� 
		if (NULL == receiveThread) {
			printf("\nCreatThread AnswerThread() failed.\n");
		}
		else{
			printf("\nCreate Receive Client Thread OK.\n");
		}
		ReleaseSemaphore(bufferMutex, 1, NULL);		// V����Դռ����ϣ�
	}
	HANDLE sendThread = CreateThread(NULL, 0, SendMessageThread, NULL, 0, NULL);
	WaitForSingleObject(sendThread, INFINITE);  // �ȴ��߳̽���
	CloseHandle(sendThread);
	CloseHandle(bufferMutex);
	WSACleanup();	// ��ֹ���׽��ֿ��ʹ��
	printf("\n");
	system("pause");
	return 0;
}


DWORD WINAPI SendMessageThread(LPVOID IpParameter)
{
	if (!talk.empty())
		cout << "aaaa" << endl;
	else
		cout << "bbbbb" << endl;
	while (1){
		cout << "talk" << talk << endl;
		int raw = 0, col = 0;
		int i = 0;
			while (talk[i] != ' ')
			{
				raw = raw * 10 + (talk[i] - '0');
				i++;
			}
			i++;
			while (talk[i] != '*' && talk[i] != '+')
			{
				col = col * 10 + (talk[i] - '0');
				i++;
			}
			if (flag == talk[i])
			{
				cout << talk << endl << raw << endl << col << endl;
				weiqi001.move(raw, col, talk[i]);
				weiqi001.whowin(raw, col, talk[i]);
				weiqi001.show();
				if (talk[i] == '*')
				{
					talk.push_back('+');
					flag = '+';
				}
				else{
					talk.push_back('*');
					flag = '*';
				}
				//}
				talk += weiqi001.winner;
				
			}
		if (tal != talk&&(!talk.empty()))
		{
			WaitForSingleObject(bufferMutex, INFINITE);		// P����Դδ��ռ�ã�  
			/*	if("quit" == talk){
			ReleaseSemaphore(bufferMutex, 1, NULL);		// V����Դռ����ϣ�
			return 0;
			}
			else*/
			{
				talk.append("\n");
			}
			for (int i = 0; i < clientSocketGroup.size(); ++i){
				//		send(clientSocketGroup[i], talk.c_str(), talk.size(), 0);	// ������Ϣ
				send(clientSocketGroup[i], talk.c_str(), 200, 0);	// ������Ϣ
			}
			ReleaseSemaphore(bufferMutex, 1, NULL);		// V����Դռ����ϣ� 

			tal = talk;
		}
	}
	return 0;
}


DWORD WINAPI ReceiveMessageThread(LPVOID IpParameter)
{
	SOCKET ClientSocket = (SOCKET)(LPVOID)IpParameter;
	while (1){

		int ret = 0;
		char recvBuf[300] = { '\0' };
		ret=recv(ClientSocket, recvBuf, 200, 0);

		WaitForSingleObject(bufferMutex, INFINITE);		// P����Դδ��ռ�ã�  
		if (recvBuf[0] == 'q' && recvBuf[1] == 'u' && recvBuf[2] == 'i' && recvBuf[3] == 't' && recvBuf[4] == '\0'){
			vector<SOCKET>::iterator result = find(clientSocketGroup.begin(), clientSocketGroup.end(), ClientSocket);
			clientSocketGroup.erase(result);
			closesocket(ClientSocket);
			ReleaseSemaphore(bufferMutex, 1, NULL);		// V����Դռ����ϣ� 
			printf("\nAttention: A Client has leave...\n", 200, 0);
			break;
		}		
		talk = recvBuf;
			ReleaseSemaphore(bufferMutex, 1, NULL);		// V����Դռ����ϣ� 
	}
	return 0;
}