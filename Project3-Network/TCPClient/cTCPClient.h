#ifndef _cTCPClient_HG_
#define _cTCPClient_HG_


#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>

#include "cBuffer.h"
#include "sProtocols.h"

class cTCPClient
{
	cTCPClient();
	~cTCPClient();

	static cTCPClient* m_instance;
public:
	static cTCPClient& GetInstance();
	static void DeleteInstance();

	// Initialize client
	bool StartClient();
	// message loop
	bool RunClient();
	//Cleanup
	bool ShutDown();


private:

	enum eChatMenuState : int
	{
		e_Connect,
		e_StartMenu,
		e_Signin,
		e_SigninSuccess,
		e_SigninFailure,
		e_Login,
		e_LoginSuccess,
		e_LoginFailure,
		e_MainMenu,
		e_CreateRoom,
		e_CreateRoomSuccess,
		e_CreateRoomFailure,
		e_JoinRoom,
		e_JoinRoomSuccess,
		e_JoinRoomFailure,
		e_LeaveRoomSuccess,
		e_LeaveRoomFailure,
		e_ChatRoom,
		e_ChatFailure,
	};
	eChatMenuState m_menuState;
	// disaplay user interactive information
	bool InputNumber(int& outNumber);
	bool InputName(const std::string& title, unsigned int maximum_chars, std::string& outString, short CurX, short CurY, char exceptCh=0);
	bool InputPassword(const std::string& title, unsigned int maximum_chars, std::string& outString, short CurX, short CurY);
	void UserSendThread();
	void ClientReceiveTherad();
	std::thread GetSenderThread();
	std::thread GetReceiverThread();

	//Step 1
	// Initialize Winsock
	bool Initialize();
	// Resolve the server address and port
	bool GetAddressInfo(addrinfo*& outAddressInfo);
	//Step 2: Create a Socket
	bool CreateSocket(const addrinfo& addressInfo, SOCKET& newSocket);
	// Attempt to connect to an address until one succeeds
	bool ConnectToServer();

	//Step 6: Receiveing and sending data
	// reads from the incoming steam
	// Receive until the peer closes the connection
	bool ReceiveData(SOCKET& connectedSocket);
	bool PacketProcedure(SOCKET& connectedSocket);
	// writes to the outgoing stream
	bool SendData(SOCKET& connectedSocket, cBuffer& buffer);

	// shutdown the connection since no more data will be sent
	bool Disconnect(SOCKET& connectedSocket);

	void RequestLogout();
	void RequestDisconnect();

	// when console close event occur, it make sure the clean up every thing before console shutdown
	static BOOL WINAPI ConsoleHandler(DWORD CEvent);

	bool m_isPrinting;
	COORD m_inputCursorPos;
	void StartPrinting();
	void EndPrinting(bool isInput);

	void SetCursorPos(short x, short y);

	void DrawRectangle(short x, short y, short width, short height);
	void PrintPos(short x, short y, const char* txt);

	void PrintTitlePage(const std::string& title);
	void PrintLobbyUsers();
	sProtocolLobbyUsers m_lobbyUsers;

	void PrintRoomList();
	//void RequestRoomList();
	sProtocolResponseRoomList m_roomList;

	void PrintCurrentRoomUsers();
	std::string m_userName;
	sProtocolResponseRoomInfo m_currentRoomInfo;

	void PrintChatHistory();
	sProtocolResponseChatHistory m_chatHistory;

	sProtocolResponseFailure m_recvFailure;

	// one is only sender, the other is only receiver
	SOCKET m_connectedSocket;
	cBuffer m_receiveBuffer;
	cBuffer m_sendBuffer;

	bool m_isConnected;
	bool m_isStopped;
	//bool m_isSentData;

	static const char* mc_DEFAULT_PORT;
	static const char* mc_DEFAULT_IPADDRESS;

	bool m_isDebug;
};

#endif