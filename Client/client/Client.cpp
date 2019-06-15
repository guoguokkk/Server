#include "Client.h"
#include<iostream>
#include<stdio.h>
#include"../tool/CellNetWork.h"
#include"../tool/CellLog.h"

Client::Client()
{
	_isConnect = false;
}

Client::~Client()
{
	closeClient();
}

void Client::initClient(int sendSize, int recvSize)
{
	CellNetwork::Init();

	if (_pClient)
	{
		CELLLOG_INFO("<socket=%d> close old connections.", _pClient->getSockfd());// cout语句不是原子操作
		closeClient();
	}

	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSock == INVALID_SOCKET)
	{
		CELLLOG_INFO("<socket=%d> build socket error.", (int)clientSock);
	}
	else
	{
		//CELLLOG_INFO("<socket=%d> build socket success.", _client_sock);
		_pClient = new CellClient(clientSock, sendSize, recvSize);
	}
}

int Client::connectToServer(const char* ip, unsigned short port)
{
	if (_pClient == nullptr)
	{
		initClient();
	}

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
#ifdef _WIN32
	server_addr.sin_addr.S_un.S_addr = inet_addr(ip);
#else
	server_addr.sin_addr.s_addr = inet_addr(ip);
#endif // _WIN32

	int ret = connect(_pClient->getSockfd(), (sockaddr*)& server_addr, sizeof(server_addr));
	if (ret == SOCKET_ERROR)
	{
		CELLLOG_INFO("<socket=%d> connect error.", (int)_pClient->getSockfd());
	}
	else
	{
		_isConnect = true;
		//CELLLOG_INFO("<socket=%d> connect success.", _pClient->getSockfd());
	}
	return ret;
}

//关闭客户端
void Client::closeClient()
{
	if (_pClient)
	{
		delete _pClient;
		_pClient = nullptr;
	}
	_isConnect = false;
}

bool Client::onRun()
{
	if (isRun())
	{
		SOCKET clientSock = _pClient->getSockfd();

		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(clientSock, &fdRead);

		fd_set fdWrite;
		FD_ZERO(&fdWrite);

		timeval time = { 0,1 };
		int ret = 0;
		if (_pClient->needWrite())
		{
			FD_SET(clientSock, &fdWrite);
			ret = select(clientSock + 1, &fdRead, &fdWrite, 0, &time);//linux 需要+1
		}
		else
		{
			ret = select(clientSock + 1, &fdRead, 0, 0, &time);//linux 需要+1
		}

		if (ret < 0)
		{
			CELLLOG_INFO("<socket=%d> select error 1.", (int)clientSock);
			closeClient();
			return false;
		}

		if (FD_ISSET(clientSock, &fdRead))
		{
			int ret = recvData(clientSock);//处理收到的消息
			if (ret == -1)
			{
				CELLLOG_INFO("<socket=%d> select error 2.", (int)clientSock);
				closeClient();
				return false;
			}
		}

		if (FD_ISSET(clientSock, &fdWrite))
		{
			int ret = _pClient->sendDataReal();//处理收到的消息
			if (ret == -1)
			{
				CELLLOG_INFO("<socket=%d> select error 2.", (int)clientSock);
				closeClient();
				return false;
			}
		}
		return true;
	}
}

bool Client::isRun()
{
	return _pClient && _isConnect;
}

//接受服务器端数据
int Client::recvData(SOCKET clientSock)
{
	if (isRun())
	{
		int nLen = _pClient->recvData();
		if (nLen > 0)
		{
			//判断是否有消息需要处理
			if (_pClient->hasMsg())
			{
				onNetMsg(_pClient->front_msg());//处理第一个消息
				_pClient->pop_front_msg();//移除第一个数据
			}
		}
		return nLen;
	}
	return 0;
}

void Client::onNetMsg(netmsg_Header* header)
{
}

int Client::sendData(netmsg_Header* header)
{
	if (isRun())
	{
		return _pClient->sendData(header);
	}
	return 0;
}

int Client::sendData(const char* pData, int len)
{
	if (isRun())
	{
		return _pClient->sendData(pData, len);
	}
	return 0;
}
