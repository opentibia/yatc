//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////


// NOT UPDATED TO PROTOCOL 84 YET!!!!!!!

#include "../fassert.h"
#include "protocolgame84.h"

ProtocolGame84::ProtocolGame84(const std::string& accountname, const std::string& password, const std::string& name, bool isGM) :
ProtocolGame83(accountname, password, name, isGM)
{
	//
}

ProtocolGame84::~ProtocolGame84()
{
	//
}

void ProtocolGame84::checkVersion()
{
	ASSERT(ProtocolConfig::getInstance().getClientVersion() == CLIENT_VERSION_840);
}



//////
char ProtocolGame84::translateSpeakClassFromInternal(SpeakClasses_t s){
    switch(s){
        case SPEAK_SAY:
            return 0x01;
        case SPEAK_WHISPER:
            return 0x02;
        case SPEAK_YELL:
            return 0x03;
        case SPEAK_PRIVATE_PN:
            return 0x04;
        case SPEAK_PRIVATE_NP:
            return 0x05;
        case SPEAK_PRIVATE:
            return 0x06;
        case SPEAK_CHANNEL_Y:
            return 0x07;

        case SPEAK_CHANNEL_W: // 8.4 onwards
            return 0x08;

        case SPEAK_RVR_CHANNEL:
            return 0x09; // reporting rule violation - ctrl+r
        case SPEAK_RVR_ANSWER:
            return 0x0A; // answering report
        case SPEAK_RVR_CONTINUE:
            return 0x0B; // answering the answer of report

        case SPEAK_BROADCAST:
            return 0x0C;
        case SPEAK_CHANNEL_R1:
            return 0x0D;
        case SPEAK_PRIVATE_RED:
            return 0x0E;
        case SPEAK_CHANNEL_O:
            return 0x0F;
        // 0x10
        case SPEAK_CHANNEL_R2:
            return 0x11;
        // 0x12
        case SPEAK_MONSTER_SAY:
            return 0x13;
        case SPEAK_MONSTER_YELL:
            return 0x14;

        default: // UNK6, UNK7, UNK8 -- perhaps these are reports? those that we commented out above?
            RAISE_PROTOCOL_WARNING("speakclass translatefrominternal failed");
    }
    //RAISE_PROTOCOL_ERROR("speakclass translatetoint error");
}

SpeakClasses_t ProtocolGame84::translateSpeakClassToInternal(char s){
    switch(s){
        case 0x01:
            return SPEAK_SAY;
        case 0x02:
            return SPEAK_WHISPER;
        case 0x03:
            return SPEAK_YELL;
        case 0x04:
            return SPEAK_PRIVATE_PN;
        case 0x05:
            return SPEAK_PRIVATE_NP;
        case 0x06:
            return SPEAK_PRIVATE;
        case 0x07:
            return SPEAK_CHANNEL_Y;

        case 0x08: // 8.4 onwards
            return SPEAK_CHANNEL_W;

        case 0x09:
            return SPEAK_RVR_CHANNEL; // reporting rule violation - ctrl+r
        case 0x0A:
            return SPEAK_RVR_ANSWER; // answering report
        case 0x0B:
            return SPEAK_RVR_CONTINUE; // answering the answer of report

        case 0x0C:
            return SPEAK_BROADCAST;
        case 0x0D:
            return SPEAK_CHANNEL_R1;
        case 0x0E:
            return SPEAK_PRIVATE_RED;
        case 0x0F:
            return SPEAK_CHANNEL_O;
        // 0x10
        case 0x11:
            return SPEAK_CHANNEL_R2;
        // 0x12
        case 0x13:
            return SPEAK_MONSTER_SAY;
        case 0x14:
            return SPEAK_MONSTER_YELL;
        default:
            printf("speakclass translatetointernal failed\n");
            return SPEAK_SAY;

    }
    //RAISE_PROTOCOL_ERROR("speakclass translatefromint error");
}

MessageType_t ProtocolGame84::translateTextMessageToInternal(uint8_t messageType)
{
	MessageType_t nmessageType;
	switch(messageType){
		case 0x11:
			nmessageType = MSG_STATUS_CONSOLE_RED;
			break;
		case 0x12:
			nmessageType = MSG_EVENT_DEFAULT; // broadcast from raid script
			break;
		case 0x13:
			nmessageType = MSG_STATUS_CONSOLE_ORNG;
			break;
        case 0x14:
            nmessageType = MSG_STATUS_CONSOLE_ORNG2;
            break;
		case 0x15:
			nmessageType = MSG_STATUS_WARNING;
			break;
		case 0x16:
			nmessageType = MSG_EVENT_ADVANCE;
			break;
		case 0x17:
			nmessageType = MSG_EVENT_DEFAULT;
			break;
		case 0x18:
			nmessageType = MSG_STATUS_DEFAULT;
			break;
		case 0x19:
			nmessageType = MSG_INFO_DESCR;
			break;
		case 0x1A:
			nmessageType = MSG_STATUS_SMALL;
			break;
		case 0x1B:
			nmessageType = MSG_STATUS_CONSOLE_BLUE;
			break;

		default:
			/*RAISE_PROTOCOL_ERROR(*/
			printf("text message - 8.4 translation problem\n");
			nmessageType = MSG_INFO_DESCR;
	}
	return nmessageType;
}
