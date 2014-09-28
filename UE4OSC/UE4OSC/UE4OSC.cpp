// UE4OSC created by Zach Duer
// see readme for copyright, permissions and credits
// oscpack created by Ross Bencina, see oscpack files for applicable copyright and permissions

#include "UE4OSC.h"
#include <stdio.h>
#include <assert.h>

#include <string>
#include <sstream>
#include <iostream>
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "ip/IpEndpointName.h"
#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"

#include <stdlib.h>
#include "atlbase.h"
#include "atlstr.h"
#include "comutil.h"

#include <windows.h>
#include <process.h>
#include <math.h>

#include <map> 
#include <windows.h>

#include <Ws2tcpip.h>

#define C_DATA_SIZE	30000000
#define OUTPUT_BUFFER_SIZE 2048

namespace osc{

	class PacketListener : public osc::OscPacketListener {
	protected:

		virtual void ProcessMessage(const osc::ReceivedMessage& m,
			const IpEndpointName& remoteEndpoint)
		{
			try{
				
				ReceivedMessageArgumentStream args = m.ArgumentStream();

				if (strcmp(m.AddressPattern(), "/Float") == 0)
				{
					float tempfloat;
					args >> tempfloat >> osc::EndMessage;
					MyFloat = tempfloat;
				}
				else if (strcmp(m.AddressPattern(), "/Array") == 0)
				{
					MyArraySize = m.ArgumentCount();
					osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
					int i;
					MyFloatArray = new float[MyArraySize]; // this is a memory leak because i never delete this allocated memory
					for (i = 0; i<MyArraySize; i++)
					{
						MyFloatArray[i] = (arg++)->AsFloat();
					}
				}
				else
				{
					std::cout << "unrecognised address pattern: "
						<< m.AddressPattern() << "\n";
				}
			}
			catch (osc::Exception& e){
				// any parsing errors such as unexpected argument types, or 
				// missing arguments get thrown as exceptions.
				std::cout << "error while parsing message: "
					<< m.AddressPattern() << ": " << e.what() << "\n";
			}
		}
	};

}	// end namespace osc


// ======================= DLL GET FUNCTIONS (called from UE4 to retrieve the most recent messages from an external program) =====================================
__declspec(dllexport)float GetOSCFloat()
{
	return MyFloat;
}

__declspec(dllexport)float * GetOSCFloatArray()
{
	return MyFloatArray;
}

__declspec(dllexport)int GetOSCArraySize()
{
	return MyArraySize;
}

// ======================= DLL SEND FUNCTIONS (Called from UE4 to send messages to external program) =====================================
__declspec(dllexport)void SendOSCStringMessage(char* AddressPattern, char* Message, char* Host, int SendPort)
{
	int test1 = 100;
	float test2 = 222;

	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
	UdpTransmitSocket socket(IpEndpointName(Host, SendPort));
	p.Clear();

	p << osc::BeginMessage("/player")
		<< (int)test1
		<< (float)test2
		<< osc::EndMessage;

	if (p.IsReady()){ socket.Send(p.Data(), p.Size()); }
}

__declspec(dllexport)void SendOSCFloat(char* AddressPattern, float FloatMsg, char* Host, int SendPort)
{
	//char* AddressPattern = "/penis";

	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
	UdpTransmitSocket socket(IpEndpointName(Host, SendPort));
	p.Clear();

	//p << osc::BeginMessage("/UE4_Float")
	p << osc::BeginMessage(AddressPattern)
		<< (float)FloatMsg
		<< osc::EndMessage;

	if (p.IsReady()){ socket.Send(p.Data(), p.Size()); }
}

// ======================= OSC STRUCTURAL FUNCTIONS =====================================
__declspec(dllexport) void StartOSCReceiver(int InReceivePort)
{
	/**
	* lets the user set ReceivePort, a global variable, so that it can be used by function ReceiveOSCMessage
	* which is used in the constructor for handleT1
	*/
	ReceivePort = InReceivePort; 

	double *myData = (double*)malloc(sizeof(double)* C_DATA_SIZE);

	HANDLE handleT1;
	unsigned int myThreadId;
	handleT1 = (HANDLE)_beginthreadex(
		NULL,                 // security
		0,			// Stack size, 0 is usually fine.
		ReceiveOSCMessage,		// This is the function that will be executed
		(void*)myData,	// Argument list. In this case, a pointer to our data array
		0,			// 0 will start thread immediately. See MSDN for more info
		&myThreadId);	// Thread ID. This can be NULL, if you'd like.
}

__declspec(dllexport) void StopOSCReceiver()
{
	//    void Run() { mux_.Run(); }
	//void RunUntilSigInt() { mux_.RunUntilSigInt(); }
	// void Break() { mux_.Break(); }
}

//void receiveOSCmessage()
unsigned int __stdcall ReceiveOSCMessage(void *arg)
{
	osc::PacketListener listener;
	
	UdpListeningReceiveSocket s(
		IpEndpointName(IpEndpointName::ANY_ADDRESS, ReceivePort),
		&listener);
		
	s.RunUntilSigInt();
	
	return 0;
}

// ======================= MISCELLANEOUS FUNCTIONS =====================================



