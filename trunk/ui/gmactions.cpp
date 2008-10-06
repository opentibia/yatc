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


#include "gmactions.h"
UIGMActions::UIGMActions(){
/*
    reason[CLIENT_VERSION_770][0]="Offensive name";
    reason[CLIENT_VERSION_770][1]="Name containing part of sentence";
    reason[CLIENT_VERSION_770][2]="Name with nonsensical letter combo";
    reason[CLIENT_VERSION_770][3]="Invalid name format"; break;
    reason[CLIENT_VERSION_770][4]="Name not describing person"; break;
    reason[CLIENT_VERSION_770][5]="Name of celebrity"; break;
    reason[CLIENT_VERSION_770][6]="Name referring to country"; break;
    reason[CLIENT_VERSION_770][7]="Name to fake player identity"; break;
    reason[CLIENT_VERSION_770][8]="Name to fake official position"; break;
    reason[CLIENT_VERSION_770][9]="Offensive statement"; break;
    reason[CLIENT_VERSION_770][10]="Spamming"; break;
    reason[CLIENT_VERSION_770][11]="Advertisement not related to game"; break;
    reason[CLIENT_VERSION_770][12]="Real money advertisement"; break;
    reason[CLIENT_VERSION_770][13]="Non-English public statement"; break;
    reason[CLIENT_VERSION_770][14]="Off-topic public statement"; break;
    reason[CLIENT_VERSION_770][15]="Inciting rule violation"; break;
    reason[CLIENT_VERSION_770][16]=Reason = "Bug abuse"; break;
    reason[CLIENT_VERSION_770][17]="Game weakness abuse"; break;
    reason[CLIENT_VERSION_770][18]="Macro use"; break;
    reason[CLIENT_VERSION_770][19]="Using unofficial softweare to play"; break;
    reason[CLIENT_VERSION_770][20]="Hacking"; break;
    reason[CLIENT_VERSION_770][21]="Multi-clienting"; break;
    reason[CLIENT_VERSION_770][22]="Account trading"; break;
    reason[CLIENT_VERSION_770][23]="Account sharing"; break;
    reason[CLIENT_VERSION_770][24]="Threatening gamemaster"; break;
    reason[CLIENT_VERSION_770][25]="Pretending to have official position"; break;
    reason[CLIENT_VERSION_770][26]="Pretending to have influence on gamemaster"; break;
    reason[CLIENT_VERSION_770][27]="False report to gamemaster"; break;
    reason[CLIENT_VERSION_770][28]="Excessive unjustifed player killing"; break;
    reason[CLIENT_VERSION_770][29]="Destructive behaviour"; break;
    reason[CLIENT_VERSION_770][30]="Spoiling auction"; break;
    reason[CLIENT_VERSION_770][31]="Invalid payment"; break;
    reason[CLIENT_VERSION_770][-1]="Unknown Reason"; break;
    action[CLIENT_VERSION_770][0]="Notation";
    action[CLIENT_VERSION_770][1]="Namelock";
    action[CLIENT_VERSION_770][2]="AccountBan";
    action[CLIENT_VERSION_770][3]="Namelock/AccountBan";;
    action[CLIENT_VERSION_770][4]="AccountBan+FinalWarning";
    action[CLIENT_VERSION_770][5]="Namelock/AccountBan+FinalWarning";
    action[CLIENT_VERSION_770][-1]="UnknownAction";
*/


    reason[CLIENT_VERSION_810][0]="Offensive Name";
    reason[CLIENT_VERSION_810][1]="Invalid Name Format";
	reason[CLIENT_VERSION_810][2]="Unsuitable Name";
	reason[CLIENT_VERSION_810][3]="Name Inciting Rule Violation";
	reason[CLIENT_VERSION_810][4]="Offensive Statement";
	reason[CLIENT_VERSION_810][5]="Spamming";
	reason[CLIENT_VERSION_810][6]="Illegal Advertising";
	reason[CLIENT_VERSION_810][7]="Off-Topic Public Statement";
	reason[CLIENT_VERSION_810][8]="Non-English Public Statement";
	reason[CLIENT_VERSION_810][9]="Inciting Rule Violation";
	reason[CLIENT_VERSION_810][10]="Bug Abuse";
	reason[CLIENT_VERSION_810][11]="Game Weakness Abuse";
	reason[CLIENT_VERSION_810][12]="Using Unofficial Software to Play";
	reason[CLIENT_VERSION_810][13]="Hacking";
	reason[CLIENT_VERSION_810][14]="Multi-Clienting";
	reason[CLIENT_VERSION_810][15]="Account Trading or Sharing";
	reason[CLIENT_VERSION_810][16]="Threatening Gamemaster";
	reason[CLIENT_VERSION_810][17]="Pretending to Have Influence on Rule Enforcement";
	reason[CLIENT_VERSION_810][18]="False Report to Gamemaster";
	reason[CLIENT_VERSION_810][19]="Destructive Behaviour";
	reason[CLIENT_VERSION_810][20]="Excessive Unjustified Player Killing";
	reason[CLIENT_VERSION_810][21]="Invalid Payment";
	reason[CLIENT_VERSION_810][22]="Spoiling Auction";
	reason[CLIENT_VERSION_810][-1]="Unknown Reason";
    action[CLIENT_VERSION_810][0]="Notation";
	action[CLIENT_VERSION_810][1]="Name Report";
	action[CLIENT_VERSION_810][2]="Banishment";
	action[CLIENT_VERSION_810][3]="Name Report + Banishment";
	action[CLIENT_VERSION_810][4]="Banishment + Final Warning";
	action[CLIENT_VERSION_810][5]="Name Report + Banishment + Final Warning";
	action[CLIENT_VERSION_810][6]="Statement Report";
	action[CLIENT_VERSION_810][7]="Deletion";
}

const std::string& UIGMActions::getAction(ClientVersion_t version, uint8_t id) {
    static std::string notfound("Unknown Action");

    if (action.find(version) == action.end())
        return notfound;
    else if (action[version].find(id) == action[version].end())
        return notfound;
    else
        return action[version][id];
}

const std::string& UIGMActions::getReason(ClientVersion_t version, uint8_t id) {
    static std::string notfound("Unknown Reason");
    if (reason.find(version) == reason.end())
        return notfound;
    else if (reason[version].find(id) == reason[version].end())
        return notfound;
    else
        return reason[version][id];
}
