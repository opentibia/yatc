CXXFLAGS=-DCLI_ONLY -DLOGIN_ONLY -DHAVE_GMP_H=1
CFLAGS=-DCLI_ONLY -DLOGIN_ONLY -DHAVE_GMP_H=1
OBJS=yatc_cli.o ../net/connection.o ../net/networkmessage.o ../debugprint.o ../util.o notifications.o ../gamecontent/creature.o ../gamecontent/container.o ../gamecontent/globalvars.o ../gamecontent/inventory.o ../gamecontent/map.o creatureui.o thingui.o distanceui.o effectui.o objects.o ../net/protocolconfig.o ../net/rsa.o ../net/encryption.o ../net/protocollogin.o
yatc_cli: $(OBJS)
	g++ $(OBJS) -lSDL -lgmp -o yatc_cli
