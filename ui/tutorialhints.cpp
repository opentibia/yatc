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

#ifdef HAVE_CONFIG_H
	#include "../config.h"
#endif
#if defined(HAVE_LIBINTL_H)
	#include <libintl.h>
#else
	#define gettext(x) (x)
#endif
#include "tutorialhints.h"
#include "../product.h"
#include "../util.h"
UITutorialHints::UITutorialHints() {

    // The following text titled "Tutorial Hints" is written by Ivan Vucica.
    // Tutorial hints are currently also licensed under GNU General Public License.
    // That means they currently may not be used in non-GPL software.
    // That also means if you modify the text, you must also grant others the
    //  same right to modify the text, to redistribute it, and to see its source code.
    //  And you must not use it in a non-GPL client.
    //
    //
    // Note, the following text is completely and only under the copyright below,
    //  and licensed as below. No matter what other portions of the program (e.g.
    //  README) may say, the text called "Tutorial Hints" is only copyrighted by
    //  Ivan Vucica, and not by, for example, the OpenTibia Team.
    //
    // This copyright and license only extends to the text inside quotes below.
    // Anything outside quotes is covered by standard license and copyright of
    // the program (which due to requirements of the GPL also has to be GPL).
    //
    // License for "Tutorial Hints" text follows.
    //
    // Tutorial Hints
    // Copyright (c) 2008 Ivan Vucica.
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
    //
    //
    // This text will remain under this license until a more suitable one
    // is selected, e.g. GFDL.


    m_tutorialhints[1] = str_replace("$$PRODUCTSHORT$$", PRODUCTSHORT, gettext(
        "Welcome to $$PRODUCTSHORT$$! Use {arrows} or {mouse} to move around."
        ));
    m_tutorialhints[2] = gettext(
        "Ok; you can also try {clicking} on the {minimap}. For example \n"
        "there's a {marker} on the minimap now, try moving there."
        );
    m_tutorialhints[3] = gettext(
        "<Observe the quest button>"
        );
    m_tutorialhints[4] = gettext(
        "There's a {chest} in here. So try {ctrl+clicking} (commonly called \n"
        "'Use') on it to open it."
        );
    m_tutorialhints[5] = gettext(
        "Now you've got a {bag} to carry stuff in and there's already a {coat}\n"
        "in there. {Drag} the coat from the {bag} to the {armor slot} to wear it."
        );
    m_tutorialhints[6] = gettext(
        "To lit the {torch}, try {ctrl+clicking} on it. Torches burn out slowly \n"
        "while they're on, so don't forget to {ctrl+click} (use) it when you don't\n"
        "need it. Many things can be ctrl+clicked in this game, so try looking around."
        );
    m_tutorialhints[7] = gettext(
        "<Observe the battle window>"
        );
    m_tutorialhints[8] = gettext(
        "Why fight if you can't profit, right? So when you kill something, just loot \n"
        "its {body}. Just do a {ctrl+click} on it."
        );
    m_tutorialhints[9] = gettext(
        "So the {container window} opened. {Drag} whatever's in it into your {bag} to \n"
        "take it with you. You need to collect three cockroach legs for the NPC!"
        );
    m_tutorialhints[10] = gettext(
        "The {shovel}, which is now in your inventory, is used to dig stuff up. It's \n"
        "very useful for exploring new areas. So go to the east, and you should find a \n"
        "{loose stone pile}, try {ctrl+clicking} on the {shovel} and then on the {stones}.\n"
        "When you use an item on another item, it's called extended using."
        );
    m_tutorialhints[11] = gettext(
        "With the {rope} you can climb up from dungeons and caves. Find a {rope spot}, \n"
        "{ctrl+click} on the {rope}, and then on the {rope spot}."
        );
    m_tutorialhints[12] = gettext(
        "<The outfit window should be opened now>"
        );
    m_tutorialhints[13] = gettext(
        "Ah, the all-mighty trade window. This hint is not written yet."
        );
    m_tutorialhints[14] = gettext(
        "Welcome to the village! There are a few more marks on your map now with important\n"
        "locations and NPCs. You're now supposed to fight until level 8, when you can \n"
        "choose a vocation and go to the mainland."
        );
    m_tutorialhints[15] =
        ""; // empty text; do not display
    m_tutorialhints[16] = gettext(
        "Looks like you've met a marked player. See the skull? That means that guy fought \n"
        "with someone. The {white skull} means he just tried to fight with someone a few \n"
        "minutes ago, and {red skull} means he killed several people."
        );
    m_tutorialhints[17] =
        ""; // empty text; do not display
    m_tutorialhints[18] = gettext(
        "<Observe the follow button>"
        );
    m_tutorialhints[19] = gettext(
        "<Observe the health bar>"
        );

}

const std::string& UITutorialHints::getTutorialHint(uint8_t id) {
    static std::string notfound("Tutorial hint not found");
    if (m_tutorialhints.find(id) != m_tutorialhints.end())
        return m_tutorialhints[id];
    else
        return notfound;
}

// To look at objects such as this sign, right-click on them and select 'Look'. Sometimes you have to walk a bit closer to signs.

// You see a sign.
// You read: To the village

// Now continue to the next mark on your automap to the east. You can point your mouse cursor on a mark to read its name.

// To go up stairs or ramps like this one, simply walk on them.

// This is Santiago, a Non-Player Character. You can chat with NPCs by typing 'Hi' or 'Hello'. Walk to Santiago and try it!

// Santiago: Don't be shy, can't hurt to greet me with 'hello' or 'hi'!
// Santiago: Hey! You over there, could you help me with a little quest? Just say 'hi' or 'hello' to talk to me!
// Santiago: Nasty creepy crawlies!

// Santiago: Hello $$CHARNAME$$, nice to see you on Rookgaard! I saw you walking by and wondered if you could help me. Could you? Please, say {yes}!

// Santiago: Ah, thank you so much. To keep talking to me, either use the {dark} {blue} keywords or answer with '{yes}' or '{no}'. Listen, I have a little cockroach problem, but ... SOMETHING SOMETHING (didnt fit in yatc screen)

// etc...

// Santiago: Good bye traveller, and enjoy your stay on Rookgaard.

// This is Santiago's cellar. You have no business there yet.
// This is Santiago's room. Maybe you should talk to him before sniffing around in his house.

// You have found a bag.

// Santiago: Evil little beasts... I hope someone helps me fight them.

// You can't see any cockroaches here. 'Open' this chest and see if you can find something to light the room better.

// You have found a torch.

// Look at the metallic object on the floor - this is a sewer gate. Right-click on it and select 'Use' to climb down.

// You smell stinky cockroaches around here. When you see one, walk to it and attack it by left-clicking it in your battle list!

// You can also automatically chase after selected creatures by clicking the 'chase opponent' button near your inventory.

// Well done, you have enough cockroach legs! You should head back to Santiago with them. Climb the ladder to the north to exit.

// Have you already climbed up a ladder? Right-click on its lower right end and select 'Use'.

// Well done, and don't forget - you can 'Use' and 'Look' at many objects in Tibia.

// If your torch is still lit when you leave the cellar, you can turn it off again to save its power for darker regions.

// You advanced from Level 1 to Level 2!

// Good job! For that, I'll grant you 100 experience points! Oh - what was that? I think you advanced a level, {right}?

// This is not the way to the forest. You should head southwest first.

// etc .... sigh there's loads of it.

// You are not ready to enter the village of Rookgaard  yet. You should talk to Carlos some more.
